#include "stdafx.h"
#include "Watchful.h"

#include "Component/PointMoveSystem.h"

Watchful::Watchful(const WatchfulDesc & desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&Watchful::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&Watchful::Update, this);
	FlowTesk::FuncCancel = bind(&Watchful::Cancel, this);
}

Watchful::~Watchful()
{
}

void Watchful::Call(const FutureAction * action)
{
	result.SetAction(action);
	desc.Anim->Play(*desc.IdleClipNum);
}

void Watchful::Update()
{
	//float lengthSq;
	//desc.Target->PreUpdate(*desc.Focus, &lengthSq);
}

void Watchful::Cancel()
{
	result.Clear();
}
