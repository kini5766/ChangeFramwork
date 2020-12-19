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
	doubleClickTime = GetDoubleClickTime();  // 제어판에서 지정한 시간 
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
	UINT curTime = (UINT)Time::Get()->Running();  // 현재 프레임 시각
	for (int i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		// 일반 클릭처리 갱신
		buttonStatus[i][1] = buttonStatus[i][0];

		// 더블 클릭처리 갱신
		// 저번 프레임에서 더블 클릭이 적용 됬다면 입력 대기상태로
		if (DoubleClick(i))
			buttonStatus[i][2] = 0;

		// 더블클릭 입력시간 초과시 입력 대기 상태로
		if (curTime - startDblClk[i] > doubleClickTime)
			buttonStatus[i][2] = 0;

		// 클릭 카운트 추가
		if (Down(i))
		{
			buttonStatus[i][3] = buttonStatus[i][2];
			buttonStatus[i][2] = true;
		}
	}

	// 현재 프레임에 버튼 클릭했는지를 저장
	buttonStatus[0][0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? true : false; // 왼쪽 버튼
	buttonStatus[1][0] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? true : false; // 오른쪽 버튼
	buttonStatus[2][0] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? true : false; // 가운데 버튼

	// 값 세팅
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
			// 이전 위치와 현재 위치가 같지 않다면
			// 더블클릭 실패 적용
			for (int i = 0; i < 8; i++)
				buttonStatus[i][4] = false;
		}

		// 마우스 좌표 갱신
		position.x = newX;
		position.y = newY;
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;  // 이전 프레임 위치
		wheelStatus.z += (float)tWheelValue;  // 현재 프레임 위치
	}

	return TRUE;
}
