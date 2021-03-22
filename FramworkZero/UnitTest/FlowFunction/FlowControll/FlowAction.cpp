#include "stdafx.h"
#include "../FlowFunction.h"

FlowAction::FlowAction(const FutureAction & action)
	: action(action)
{
}

FlowAction::~FlowAction()
{
}

void FlowAction::Call(const FutureAction * result)
{
	action();
	(*result)();
}
