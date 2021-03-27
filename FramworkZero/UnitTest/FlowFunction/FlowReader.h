#pragma once


/*
FlowTesk���� �̸� ����ϰ�
����->���� ��� ������� �Լ��� ȣ�� (��������)
��� �Լ��� �����ϰ��� DelayReturn�� ȣ��
*/
class FlowReader : public IFlowTesk
{
public:
	FlowReader();
	virtual ~FlowReader();

public:
	// �Լ� ���
	void PushBack(IFlowTesk* tesk);
	// �Լ� ���(�迭)
	void PushBacks(UINT size, IFlowTesk** first);
	// ��� ����� �Լ� ����, ���� ���� �ൿ ���
	void Reset();
	// ���� �Լ��� ��� ��Ű�� 
	// ���� �Լ� �����ϰų� ������ ����ȣ��
	void CancelNext();
	// ���� �Լ��� �����ϰ� ���� �Լ��� ����
	void HoldBackNext();


public:
	// ��ϵ� ��� �Լ��� ���� �Ϸ� �� ����ȣ��
	virtual void Call(const FutureAction* action = nullptr) override;
	// ��ϵ� �Լ� ���� �������� ����
	virtual void Update() override;
	// ��ϵ� ��� �Լ� ���� �� ���� ���� �Լ� Cancel
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
