#include "stdafx.h"
#include "../FlowFunction.h"

FlowLoop::FlowLoop()
	: funcReset(bind(&FlowLoop::Reset, this))
{
	reader = new FlowReader();
}

FlowLoop::~FlowLoop()
{
	SafeDelete(reader);
}

void FlowLoop::Call(const FutureAction * action)
{
	result.SetAction(action);
	reader->Call(&funcReset);
}

void FlowLoop::Update()
{
	if (judgment() == false)
	{
		result.OnAction();
		return;
	}

	reader->Update();
}

void FlowLoop::Hold()
{
	result.Clear();
	reader->Hold();
}

void FlowLoop::Reset()
{
	reader->Reset();
	if (tesks.size() == 0)
		return;

	reader->PushBacks(tesks.size(), tesks.data());
	reader->Call(&funcReset);
}
