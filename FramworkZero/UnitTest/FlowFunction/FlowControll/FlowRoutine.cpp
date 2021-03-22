#include "stdafx.h"
#include "../FlowFunction.h"


FlowRoutine::FlowRoutine()
{
	FlowTesk::FuncCall = bind(&FlowRoutine::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&FlowRoutine::Update, this);
	FlowTesk::FuncCancel = bind(&FlowRoutine::Cancel, this);

	reader = new FlowReader();
}

FlowRoutine::~FlowRoutine()
{
	SafeDelete(reader);
}

void FlowRoutine::Call(const FutureAction * action)
{
	reader->Clear();
	if (tesks.size() == 0)
		return;

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
