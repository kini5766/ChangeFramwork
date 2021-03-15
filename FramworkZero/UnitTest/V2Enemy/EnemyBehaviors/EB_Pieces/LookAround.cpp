#include "stdafx.h"
#include "LookAround.h"


LookAround::LookAround(const LookAroundDesc & desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&LookAround::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&LookAround::Update, this);
	FlowTesk::FuncCancel = bind(&LookAround::Cancel, this);
}

LookAround::~LookAround()
{
}

void LookAround::Call(const FutureAction * action)
{
	desc.Anim->Play(*desc.ClipLookAround);
	runningTime = 0.0f;

	result.SetAction(action);
}

void LookAround::Update()
{
	if (runningTime >= *desc.LookAroundTime)
	{
		result.OnAction();
		return;
	}

	runningTime += Time::Delta();
}

void LookAround::Cancel()
{
	result.Clear();
}
