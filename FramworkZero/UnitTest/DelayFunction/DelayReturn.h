#pragma once

/*
null 체크 + OnAction 시 action값 null로 설정
*/
class DelayReturn
{
public:
	DelayReturn();
	~DelayReturn();

public:
	void SetAction(const ReturnAction* value = nullptr);
	void Clear() { action = nullptr; }
	bool IsValid() { return action != nullptr; }

	// action을 null로 바꾼 후 action에 있던 함수를 호출
	// action에 있던 함수 호출 시 SetAction함수 호출 대비
	void OnAction();

private:
	const ReturnAction* action;
};
