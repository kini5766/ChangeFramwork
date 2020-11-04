#pragma once

#define MAX_INPUT_MOUSE 8

/*
 ���콺 ��ư ����
 0 : ���� Ŭ��, 1 : ������ Ŭ��, 2 : ���콺�� Ŭ��

 ����Ŭ�� ���� �ܰ�
 1. �Է� ���
 2. 1��° Down
 3. 2��° Down -> ����Ŭ�� ����

 ����Ŭ�� �Է� ��� �ܰ�� �ǵ��ư��� ����
 1. ���� �������� ����Ŭ�� ������ ���
 2. ���� ������ ������ ���� Ŭ�� ���� �ð� �ʰ�
 3. ���콺�� �̵� ��
 */
class Mouse
{
public:
	void SetHandle(HWND handle)
	{
		this->handle = handle;
	}

	static Mouse* Get();

	static void Create();
	static void Delete();

	void Update();
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);


public:
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetMoveValue() { return wheelMoveValue; }

	bool Down(int key) { return buttonStatus[key][4]; }
	bool Up(int key) { return buttonStatus[key][5]; }
	bool Press(int key) { return buttonStatus[key][6]; }
	bool DoubleClick(int key) { return buttonStatus[key][7]; }

private:
	Mouse();
	~Mouse();

	static Mouse* instance;

	HWND handle;
	D3DXVECTOR3 position; //���콺 ��ġ

	// [key][0] : ���� ������ ���� ���� (������ 1, ���� 0)
	// [key][1] : ���� ������ ���� ���� (������ 1, ���� 0)
	// [key][2, 3] : ����Ŭ�� ���� (���� 1 & 1, ����� 0 & 1, ���� x & 0)
	bitset<MAX_INPUT_MOUSE> buttonStatus[4];

	// ���� �����ǿ��� ������ ����Ŭ�� ����
	DWORD doubleClickTime;
	// 1�� ° ���콺 Ŭ���� ������ �ð�
	DWORD startDblClk[MAX_INPUT_MOUSE];

	D3DXVECTOR3 wheelStatus;
	D3DXVECTOR3 wheelOldStatus;
	D3DXVECTOR3 wheelMoveValue;

private:
	// ���� �Լ�
	void Down(int key, bool value) { buttonStatus[key][4] = value; }
	void Up(int key, bool value) { buttonStatus[key][5] = value; }
	void Press(int key, bool value) { buttonStatus[key][6] = value; }
	void DoubleClick(int key, bool value) { buttonStatus[key][7] = value; }

};

