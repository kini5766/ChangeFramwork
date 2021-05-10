#pragma once

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Hold�ϰ� �Ǹ� �������� ����
*/
class IFlowTesk
{
public:
	// �� �ൿ�� ���õǾ� ������
	virtual void Call(const FutureAction* future) = 0;

	// �� �ൿ ���� �� ������ ȣ��
	virtual void Update() {};

	// �Ͻ� �����ǰ� �ٸ� �ൿ�� ����� ��� ȣ��
	virtual void Hold() {};
};
