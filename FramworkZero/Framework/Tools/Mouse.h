#pragma once

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
class IMouse
{
public:
	virtual D3DXVECTOR3 GetPosition() = 0;
	virtual D3DXVECTOR3 GetMoveValue() = 0;

	virtual bool Down(int key) = 0;
	virtual bool Up(int key) = 0;
	virtual bool Press(int key) = 0;
	virtual bool DoubleClick(int key) = 0;
};

class Mouse : public IMouse
{
public:
	enum
	{
		MOUSE_ROTATION_LEFT = 0,
		MOUSE_ROTATION_RIGHT,
		MOUSE_ROTATION_WHEEl,
		MAX_INPUT_MOUSE
	};

public:
	Mouse();
	~Mouse();

public:
	D3DXVECTOR3 GetPosition() override { return position; }
	D3DXVECTOR3 GetMoveValue() override { return wheelMoveValue; }

	bool Down(int key) override { return buttonStatus[key][4]; }
	bool Up(int key) override  { return buttonStatus[key][5]; }
	bool Press(int key) override { return buttonStatus[key][6]; }
	bool DoubleClick(int key) override { return buttonStatus[key][7]; }

public:
	void Update();
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetHandle(HWND value) { handle = value; }

private:
	// ���� �Լ�
	void Down(int key, bool value) { buttonStatus[key][4] = value; }
	void Up(int key, bool value) { buttonStatus[key][5] = value; }
	void Press(int key, bool value) { buttonStatus[key][6] = value; }
	void DoubleClick(int key, bool value) { buttonStatus[key][7] = value; }

private:
	HWND handle;
	D3DXVECTOR3 position; //���콺 ��ġ

	// [key][0] : ���� ������ ���� ���� (������ 1, ���� 0)
	// [key][1] : ���� ������ ���� ���� (������ 1, ���� 0)
	// [key][2, 3] : ����Ŭ�� ���� (���� 1 & 1, ����� 0 & 1, ���� x & 0)
	bitset<8> buttonStatus[MAX_INPUT_MOUSE];

	// ���� �����ǿ��� ������ ����Ŭ�� ����
	DWORD doubleClickTime;
	// 1�� ° ���콺 Ŭ���� ������ �ð�
	DWORD startDblClk[MAX_INPUT_MOUSE];

	D3DXVECTOR3 wheelStatus;
	D3DXVECTOR3 wheelOldStatus;
	D3DXVECTOR3 wheelMoveValue;
};

