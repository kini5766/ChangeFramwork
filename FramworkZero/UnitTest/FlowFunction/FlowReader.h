#pragma once


/*
FlowTesk들을 미리 등록하고
나중->먼저 등록 순서대로 함수를 호출 (스택형식)
모든 함수를 실행하고나면 DelayReturn을 호출
*/
class FlowReader : public IFlowTesk
{
public:
	FlowReader();
	virtual ~FlowReader();

public:
	// 함수 등록
	void PushBack(IFlowTesk* tesk);
	// 함수 등록(배열)
	void PushBacks(UINT size, IFlowTesk** first);
	// 모든 등록한 함수 비우기, 실행 중인 행동 취소
	void Reset();
	// 현재 함수를 취소 시키고 
	// 다음 함수 실행하거나 없으면 리턴호출
	void CancelNext();
	// 현재 함수를 보류하고 다음 함수를 실행
	void HoldBackNext();


public:
	// 등록된 모든 함수들 실행 완료 후 리턴호출
	virtual void Call(const FutureAction* action = nullptr) override;
	// 등록된 함수 스택 형식으로 실행
	virtual void Update() override;
	// 등록된 모든 함수 제거 후 실행 중인 함수 Cancel
	virtual void Hold() override;

private:
	void Next();
	FutureAction funcNext;

private:
	IFlowTesk* curr = nullptr;
	IFlowTesk* next = nullptr;
	vector<IFlowTesk*> funcStack;

	FutureReturn result;
};
