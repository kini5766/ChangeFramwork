#pragma once


/*
IDelayFunction들을 미리 등록하고
나중->먼저 등록 순서대로 함수를 호출 (스택형식)
모든 함수를 실행하고나면 DelayReturn을 호출
*/
class FlowPlayer : public FlowTesk
{
public:
	FlowPlayer();
	virtual ~FlowPlayer();

public:
	// 함수 등록
	void PushBack(FlowBase* function);
	// 남은 함수들을 비움 (현재 함수는 그대로)
	void Clear();
	// 현재 함수를 취소 시키고 
	// 다음 함수 실행하거나 없으면 리턴호출
	void CancelNext();
	// 현재 함수를 보류하고 다음 함수를 실행 (Cancel 안함)
	void HoldBackNext();

	// IDelayFunction을(를) 통해 상속됨
public:
	// 등록된 모든 함수들 실행 완료 후 리턴호출
	void Call(const FutureAction* action = nullptr);
	// 등록된 함수 스택 형식으로 실행
	void Update();
	// 등록된 모든 함수 제거 후 실행 중인 함수 Cancel
	void Cancel();

private:
	void Next();
	FutureAction funcNext;

private:
	FlowTesk* curr = nullptr;
	vector<FlowBase*> funcStack;

	FutureReturn result;
};
