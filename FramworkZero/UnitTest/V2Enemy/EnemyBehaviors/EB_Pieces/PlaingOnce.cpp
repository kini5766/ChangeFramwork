#include "stdafx.h"
#include "PlaingOnce.h"

PlaingOnce::PlaingOnce(const PlaingOnceDesc & desc)
	: desc(desc)
{
	funcComple = bind(&PlaingOnce::CompleAnim, this);
	desc.Anim->AddFuncChange(desc.Clip, funcComple);
}

PlaingOnce::~PlaingOnce()
{
}


void PlaingOnce::Call(const DelayReturn * _result)
{
	result = _result;

	bChanged = false;
	desc.Anim->Play(desc.Clip);
}

void PlaingOnce::Update()
{
	if (result == nullptr)
		return;
	if (bChanged == false)
		return;

	(*result)();
	result = nullptr;
}

void PlaingOnce::Cancel()
{
	result = nullptr;
}

void PlaingOnce::CompleAnim()
{
	bChanged = true;
}
