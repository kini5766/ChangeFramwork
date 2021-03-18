#pragma once


/*
FlowTesk���� �̸� ����ϰ�
����->���� ��� ������� �Լ��� ȣ�� (��������)
��� �Լ��� �����ϰ��� DelayReturn�� ȣ��
*/
class FlowReader : public FlowTesk
{
public:
	FlowReader();
	virtual ~FlowReader();

public:
	// �Լ� ���
	void PushBack(FlowTesk* tesk);
	// �Լ� ���(�迭)
	void PushBacks(UINT size, FlowTesk** first);
	// ���� �Լ����� ��� (���� �Լ��� �״��)
	void Clear();
	// ���� �Լ��� ��� ��Ű�� 
	// ���� �Լ� �����ϰų� ������ ����ȣ��
	void CancelNext();
	// ���� �Լ��� �����ϰ� ���� �Լ��� ���� (Cancel ����)
	void HoldBackNext();

	// IDelayFunction��(��) ���� ��ӵ�
public:
	// ��ϵ� ��� �Լ��� ���� �Ϸ� �� ����ȣ��
	void Call(const FutureAction* action = nullptr);
	// ��ϵ� �Լ� ���� �������� ����
	void Update();
	// ��ϵ� ��� �Լ� ���� �� ���� ���� �Լ� Cancel
	void Cancel();

private:
	void Next();
	FutureAction funcNext;

private:
	FlowTesk* curr = nullptr;
	FlowTesk* next = nullptr;
	vector<FlowTesk*> funcStack;

	FutureReturn result;
};
