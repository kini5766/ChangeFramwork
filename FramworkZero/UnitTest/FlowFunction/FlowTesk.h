#pragma once

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Cancel�ϰ� �Ǹ� �������� ����
*/
class FlowTesk
{
public:
	// DelayReturn�� ����
	function<void(const FutureAction* future)> FuncCall;

	// �Լ� ����
	function<void()> FuncUpdate = []() {};

	// DelayReturn�� ����(DelayReturn�� ȣ�� �� ��)
	function<void()> FuncCancel = []() {};
};
