#pragma once

typedef function<void(void)> DelayReturn;

/*
함수 실행 후 바로 리턴이 되는 것이 아닌
마지막 업데이트를 마친 후 리턴
Cancel하게 되면 리턴하지 않음
*/
class IDelayFunction
{
public:
	// DelayReturn을 저장
	virtual void Call(const DelayReturn* result) = 0;
	// 함수 갱신
	virtual void Update() = 0;
	// DelayReturn를 삭제(DelayReturn를 호출 안 함)
	virtual void Cancel() = 0;
};

/*
IDelayFunction들을 미리 등록하고 
나중->먼저 등록 순서대로 함수를 호출 (스택형식)
모든 함수를 실행하고나면 DelayReturn을 호출
*/
class DelayReader : public IDelayFunction
{
public:
	DelayReader();
	virtual ~DelayReader();

public:
	// 함수 등록
	void PushBack(IDelayFunction* function);
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
	void Call(const DelayReturn* result) override;
	// 등록된 함수 스택 형식으로 실행
	void Update() override;
	// 등록된 모든 함수 제거 후 실행 중인 함수 Cancel
	void Cancel() override;

private:
	void Next();

private:
	const DelayReturn* result = nullptr;
	IDelayFunction* curr = nullptr;
	vector<IDelayFunction*> funcStack;
	DelayReturn* mineResult;
};
