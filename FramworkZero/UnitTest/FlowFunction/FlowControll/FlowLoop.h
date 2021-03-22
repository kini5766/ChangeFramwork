#pragma once

/*
judgment가 true인 동안
등록한 모든 작업 완료하면 다시 재생
일시중지 되도 순서 이어짐
*/
class FlowLoop : public IFlowTesk
{
public:
	FlowLoop();
	~FlowLoop();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;

	void SetJudgment(const Judgment& value) { judgment = value; }

private:
	void Reset();
	FutureAction funcReset;

public:
	vector<IFlowTesk*>* Tesks() { return &tesks; }

private:
	FutureReturn result;
	Judgment judgment = []() { return true; };

	FlowReader* reader;
	vector<IFlowTesk*> tesks;
};
