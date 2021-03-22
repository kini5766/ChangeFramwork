#pragma once

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Cancel�ϰ� �Ǹ� �������� ����
*/
class IFlowTesk
{
public:
	// DelayReturn�� ����
	virtual void Call(const FutureAction* future) = 0;

	// �Լ� ����
	virtual void Update() {};

	// FutureAction�� ����(FutureAction�� ȣ�� �� ��)
	virtual void Cancel() {};
};
