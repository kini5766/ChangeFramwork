#include "stdafx.h"
#include "LookAround.h"


LookAround::LookAround(const LookAroundDesc & desc)
	: desc(desc)
{
}

LookAround::~LookAround()
{
}

void LookAround::Call(const ReturnAction * action)
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
