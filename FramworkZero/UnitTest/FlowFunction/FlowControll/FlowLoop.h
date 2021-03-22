#pragma once

/*
judgment�� true�� ����
����� ��� �۾� �Ϸ��ϸ� �ٽ� ���
�Ͻ����� �ǵ� ���� �̾���
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
