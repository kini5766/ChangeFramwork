#pragma once

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Cancel�ϰ� �Ǹ� �������� ����
*/
class IDelayFunction
{
public:
	// DelayReturn�� ����
	virtual void Call(const ReturnAction* result) = 0;
	// �Լ� ����
	virtual void Update() = 0;
	// DelayReturn�� ����(DelayReturn�� ȣ�� �� ��)
	virtual void Cancel() = 0;
};