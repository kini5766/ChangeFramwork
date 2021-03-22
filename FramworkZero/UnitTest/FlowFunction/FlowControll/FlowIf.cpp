#include "stdafx.h"
#include "../FlowFunction.h"

FlowIf::FlowIf(const Judgment& judgment)
	: judgment(judgment)
	, funcResult(bind(&FlowIf::Result, this))
{
}

FlowIf::~FlowIf()
{
}

void FlowIf::Call(const FutureAction * action)
{
	currTesk = nullptr;

	if (judgment())
		currTesk = teskTrue;
	else
		currTesk = teskFalse;

	if (currTesk == nullptr)
	{
		result.Clear();
		(*action);
		return;
	}

	currTesk->Call(&funcResult);
	result.SetAction(action);
}

void FlowIf::Update()
{
	currTesk->Update();
}

void FlowIf::Cancel()
{
	result.Clear();
	currTesk = nullptr;
}

void FlowIf::Result()
{
	result.OnAction();
	currTesk = nullptr;
}
