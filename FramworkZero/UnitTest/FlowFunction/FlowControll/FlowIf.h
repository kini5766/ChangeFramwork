#pragma once


/*
ȣ�� ������ �񱳸� �ؼ� ������ ȣ������ ����
*/
class FlowIf : public IFlowTesk
{
public:
	FlowIf(const Judgment& judgment);
	virtual ~FlowIf();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Hold() override;

public:
	void SetJudgment(const Judgment& value) { judgment = value; }
	void SetTrueTesk(IFlowTesk* value) { teskTrue = value; }
	void SetFalseTesk(IFlowTesk* value) { teskFalse = value; }


private:
	Judgment judgment;
	IFlowTesk* teskTrue = nullptr;
	IFlowTesk* teskFalse = nullptr;

	IFlowTesk* currTesk = nullptr;
};