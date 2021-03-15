#pragma once


/*
단발성 함수
*/
class FlowAction : public FlowBase
{
public:
	FlowAction() { classType = FlowBase::ClassType::FlowAction; }
	function<void()> FuncCall;
};
