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
