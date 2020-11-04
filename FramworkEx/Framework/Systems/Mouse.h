#pragma once

#define MAX_INPUT_MOUSE 8

/*
 마우스 버튼 맵핑
 0 : 왼쪽 클릭, 1 : 오른쪽 클릭, 2 : 마우스휠 클릭

 더블클릭 적용 단계
 1. 입력 대기
 2. 1번째 Down
 3. 2번째 Down -> 더블클릭 적용

 더블클릭 입력 대기 단계로 되돌아가는 시점
 1. 저번 프레임이 더블클릭 상태일 경우
 2. 윈도 제어판 설정한 더블 클릭 간격 시간 초과
 3. 마우스를 이동 시
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
	D3DXVECTOR3 position; //마우스 위치

	// [key][0] : 현재 프레임 누름 상태 (누르면 1, 떼면 0)
	// [key][1] : 이전 프레임 누름 상태 (누르면 1, 떼면 0)
	// [key][2, 3] : 더블클릭 상태 (성공 1 & 1, 대기중 0 & 1, 실패 x & 0)
	bitset<MAX_INPUT_MOUSE> buttonStatus[4];

	// 윈도 제어판에서 설정한 더블클릭 간격
	DWORD doubleClickTime;
	// 1번 째 마우스 클릭이 시작한 시간
	DWORD startDblClk[MAX_INPUT_MOUSE];

	D3DXVECTOR3 wheelStatus;
	D3DXVECTOR3 wheelOldStatus;
	D3DXVECTOR3 wheelMoveValue;

private:
	// 세팅 함수
	void Down(int key, bool value) { buttonStatus[key][4] = value; }
	void Up(int key, bool value) { buttonStatus[key][5] = value; }
	void Press(int key, bool value) { buttonStatus[key][6] = value; }
	void DoubleClick(int key, bool value) { buttonStatus[key][7] = value; }

};

