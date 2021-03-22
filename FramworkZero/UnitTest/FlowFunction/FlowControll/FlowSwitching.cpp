#include "stdafx.h"
#include "../FlowFunction.h"

FlowSwitching::FlowSwitching()
{
}

FlowSwitching::~FlowSwitching()
{
}

void FlowSwitching::Call(const FutureAction * action)
{
	curr = funcCase();
	if (curr == -1)
	{
		result.Clear();
		(*action)();
		return;
	}

	result.SetAction(action);
	tesks[curr]->Call(nullptr);
}

void FlowSwitching::Update()
{
	int next = funcCase();
	if (next == -1)
	{
		result.OnAction();
		return;
	}

	if (curr != next)
	{
		curr = next;
		if (curr == -1)
			return;

		tesks[curr]->Call(nullptr);
		return;
	}

	tesks[curr]->Update();
}

void FlowSwitching::Cancel()
{
	result.Clear();
}
