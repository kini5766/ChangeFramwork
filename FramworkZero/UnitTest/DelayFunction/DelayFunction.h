#pragma once

typedef function<void(void)> DelayReturn;

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Cancel�ϰ� �Ǹ� �������� ����
*/
class IDelayFunction
{
public:
	// DelayReturn�� ����
	virtual void Call(const DelayReturn* result) = 0;
	// �Լ� ����
	virtual void Update() = 0;
	// DelayReturn�� ����(DelayReturn�� ȣ�� �� ��)
	virtual void Cancel() = 0;
};

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
	void Call(const DelayReturn* result) override;
	// ��ϵ� �Լ� ���� �������� ����
	void Update() override;
	// ��ϵ� ��� �Լ� ���� �� ���� ���� �Լ� Cancel
	void Cancel() override;

private:
	void Next();

private:
	const DelayReturn* result = nullptr;
	IDelayFunction* curr = nullptr;
	vector<IDelayFunction*> funcStack;
	DelayReturn* mineResult;
};
