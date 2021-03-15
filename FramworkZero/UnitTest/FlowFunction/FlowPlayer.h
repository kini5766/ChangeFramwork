#pragma once


/*
IDelayFunction���� �̸� ����ϰ�
����->���� ��� ������� �Լ��� ȣ�� (��������)
��� �Լ��� �����ϰ��� DelayReturn�� ȣ��
*/
class FlowPlayer : public FlowTesk
{
public:
	FlowPlayer();
	virtual ~FlowPlayer();

public:
	// �Լ� ���
	void PushBack(FlowBase* function);
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
	vector<FlowBase*> funcStack;

	FutureReturn result;
};
