#pragma once


/*
IDelayFunction���� �̸� ����ϰ�
����->���� ��� ������� �Լ��� ȣ�� (��������)
��� �Լ��� �����ϰ��� DelayReturn�� ȣ��
*/
class DelayReader : public IDelayFunction
{
public:
	DelayReader();
	virtual ~DelayReader();

public:
	// �Լ� ���
	void PushBack(IDelayFunction* function);
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
	void Call(const ReturnAction* action = nullptr) override;
	// ��ϵ� �Լ� ���� �������� ����
	void Update() override;
	// ��ϵ� ��� �Լ� ���� �� ���� ���� �Լ� Cancel
	void Cancel() override;

private:
	void Next();
	ReturnAction funcNext;

private:
	IDelayFunction* curr = nullptr;
	vector<IDelayFunction*> funcStack;

	DelayReturn result;
};
