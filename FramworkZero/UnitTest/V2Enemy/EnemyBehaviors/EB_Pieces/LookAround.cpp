#include "stdafx.h"
#include "LookAround.h"


LookAround::LookAround(const LookAroundDesc & desc)
	: desc(desc)
{
}

LookAround::~LookAround()
{
}

void LookAround::Call(const DelayReturn * _result)
{
	desc.Anim->Play(*desc.ClipLookAround);
	result = _result;
	runningTime = 0.0f;
}

void LookAround::Update()
{
	if (runningTime >= *desc.LookAroundTime)
	{
		(*result)();
		result = nullptr;
		return;
	}

	runningTime += Time::Delta();
}

void LookAround::Cancel()
{
	result = nullptr;
}
