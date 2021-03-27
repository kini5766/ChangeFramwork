#pragma once

/*
�۾����� �̸� ����ϰ�
ȣ�� ������ ó������ ���
(�Ͻ����� �Ǿ ó������)
*/
class FlowRoutine : public IFlowTesk
{
public:
	FlowRoutine();
	virtual ~FlowRoutine();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Hold() override;

public:
	vector<IFlowTesk*>* Tesks() { return &tesks; }

private:
	FlowReader* reader;
	vector<IFlowTesk*> tesks;
};
