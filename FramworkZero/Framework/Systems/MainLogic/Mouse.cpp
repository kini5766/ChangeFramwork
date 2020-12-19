#include "Framework.h"
#include "Mouse.h"

Mouse::Mouse()
{
	position = D3DXVECTOR3(0, 0, 0);

	wheelStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelOldStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelMoveValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);

	buttonStatus->reset();
	doubleClickTime = GetDoubleClickTime();  // �����ǿ��� ������ �ð� 
	startDblClk[0] = 0;

	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	UINT curTime = (UINT)Time::Get()->Running();  // ���� ������ �ð�
	for (int i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		// �Ϲ� Ŭ��ó�� ����
		buttonStatus[i][1] = buttonStatus[i][0];

		// ���� Ŭ��ó�� ����
		// ���� �����ӿ��� ���� Ŭ���� ���� ��ٸ� �Է� �����·�
		if (DoubleClick(i))
			buttonStatus[i][2] = 0;

		// ����Ŭ�� �Է½ð� �ʰ��� �Է� ��� ���·�
		if (curTime - startDblClk[i] > doubleClickTime)
			buttonStatus[i][2] = 0;

		// Ŭ�� ī��Ʈ �߰�
		if (Down(i))
		{
			buttonStatus[i][3] = buttonStatus[i][2];
			buttonStatus[i][2] = true;
		}
	}

	// ���� �����ӿ� ��ư Ŭ���ߴ����� ����
	buttonStatus[0][0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? true : false; // ���� ��ư
	buttonStatus[1][0] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? true : false; // ������ ��ư
	buttonStatus[2][0] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? true : false; // ��� ��ư

	// �� ����
	for (int i = 0; i < 8; i++)
	{
		Down(i, buttonStatus[i][0] && !buttonStatus[i][1]);
		Up(i, !buttonStatus[i][0] && buttonStatus[i][1]);
		Press(i, buttonStatus[i][0] && buttonStatus[i][1]);
		DoubleClick(i, buttonStatus[i][2] && buttonStatus[i][3]);
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(handle, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = float(point.x);
	wheelStatus.y = float(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus;
	wheelOldStatus.z = wheelStatus.z;
}

LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSEMOVE)
	{
		float newX = (float)LOWORD(lParam);
		float newY = (float)HIWORD(lParam);

		if (position.x != newX && position.y != newY)
		{
			// ���� ��ġ�� ���� ��ġ�� ���� �ʴٸ�
			// ����Ŭ�� ���� ����
			for (int i = 0; i < 8; i++)
				buttonStatus[i][4] = false;
		}

		// ���콺 ��ǥ ����
		position.x = newX;
		position.y = newY;
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;  // ���� ������ ��ġ
		wheelStatus.z += (float)tWheelValue;  // ���� ������ ��ġ
	}

	return TRUE;
}
