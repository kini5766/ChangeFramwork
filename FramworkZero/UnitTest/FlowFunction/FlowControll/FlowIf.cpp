#include "stdafx.h"
#include "../FlowFunction.h"

FlowIf::FlowIf(const Judgment& judgment)
	: judgment(judgment)
{
}

FlowIf::~FlowIf()
{
}

void FlowIf::Call(const FutureAction * action)
{
	if (judgment())
		currTesk = teskTrue;
	else
		currTesk = teskFalse;

	if (currTesk == nullptr)
	{
		(*action)();
		return;
	}

	currTesk->Call(action);
}

void FlowIf::Update()
{
	currTesk->Update();
}

void FlowIf::Cancel()
{
	if (currTesk != nullptr)
	{
		currTesk->Cancel();
		currTesk = nullptr;
	}
}
