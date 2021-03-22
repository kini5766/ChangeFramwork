#pragma once

/*
FlowTesk���� �̸� ����ϰ�
ȣ�� ������ ó������ ���
*/
class FlowRoutine : public FlowTesk
{
public:
	FlowRoutine();
	virtual ~FlowRoutine();

public:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

public:
	vector<FlowTesk*>* GetTesks() { return &tesks; }

private:
	FlowReader* reader;
	vector<FlowTesk*> tesks;
};
