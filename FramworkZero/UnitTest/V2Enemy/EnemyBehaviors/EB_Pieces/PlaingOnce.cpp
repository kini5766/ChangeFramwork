#include "stdafx.h"
#include "PlaingOnce.h"

PlaingOnce::PlaingOnce(const PlaingOnceDesc & desc)
	: desc(desc)
	, funcComple(bind(&PlaingOnce::CompleAnim, this))
{
	FlowTesk::FuncCall = bind(&PlaingOnce::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = []() {};
	FlowTesk::FuncCancel = bind(&PlaingOnce::Cancel, this);

	desc.Anim->AddFuncChange(desc.Clip, funcComple);
}

PlaingOnce::~PlaingOnce()
{
}


void PlaingOnce::Call(const FutureAction * action)
{
	result.Clear();
	result.SetAction(action);

	bChanged = false;
	desc.Anim->Play(desc.Clip);
}

void PlaingOnce::Cancel()
{
	result.Clear();
}

void PlaingOnce::CompleAnim()
{
	result.OnAction();
}
