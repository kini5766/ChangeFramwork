#pragma once


/*
�ܹ߼� �Լ�
*/
class FlowAction : public FlowBase
{
public:
	FlowAction() { classType = FlowBase::ClassType::FlowAction; }
	function<void()> FuncCall;
};
