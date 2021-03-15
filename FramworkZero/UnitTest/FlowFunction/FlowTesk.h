#pragma once

/*
�Լ� ���� �� �ٷ� ������ �Ǵ� ���� �ƴ�
������ ������Ʈ�� ��ģ �� ����
Cancel�ϰ� �Ǹ� �������� ����
*/
class FlowTesk : public FlowBase
{
public:
	FlowTesk() { classType = FlowBase::ClassType::FlowTesk; }

	// DelayReturn�� ����
	function<void(const FutureAction* future)> FuncCall;

	// �Լ� ����
	function<void()> FuncUpdate;

	// DelayReturn�� ����(DelayReturn�� ȣ�� �� ��)
	function<void()> FuncCancel;
};
