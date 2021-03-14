#include "stdafx.h"
#include "PlaingOnce.h"

PlaingOnce::PlaingOnce(const PlaingOnceDesc & desc)
	: desc(desc)
	, funcComple(bind(&PlaingOnce::CompleAnim, this))
{
	desc.Anim->AddFuncChange(desc.Clip, funcComple);
}

PlaingOnce::~PlaingOnce()
{
}


void PlaingOnce::Call(const ReturnAction * action)
{
	result.SetAction(action);

	bChanged = false;
	desc.Anim->Play(desc.Clip);
}

void PlaingOnce::Update()
{
	if (result.IsValid() == false)
		return;
	if (bChanged == false)
		return;

	result.OnAction();
}

void PlaingOnce::Cancel()
{
	result.Clear();
}

void PlaingOnce::CompleAnim()
{
	bChanged = true;
}
