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
	virtual void Hold() override;

public:
	void SetJudgment(const Judgment& value) { judgment = value; }
	vector<IFlowTesk*>* Tesks() { return &tesks; }

private:
	void Reset();
	FutureAction funcReset;


private:
	FutureReturn result;
	Judgment judgment = []() { return true; };

	FlowReader* reader;
	vector<IFlowTesk*> tesks;
};
