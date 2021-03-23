#include "stdafx.h"
#include "../FlowFunction.h"


FlowRoutine::FlowRoutine()
{
	reader = new FlowReader();
}

FlowRoutine::~FlowRoutine()
{
	SafeDelete(reader);
}

void FlowRoutine::Call(const FutureAction * action)
{
	reader->Cancel();
	if (tesks.size() == 0)
	{
		(*action)();
		return;
	}

	reader->PushBacks(tesks.size(), tesks.data());
	reader->Call(action);
}

void FlowRoutine::Update()
{
	reader->Update();
}

void FlowRoutine::Cancel()
{
	reader->Cancel();
}
