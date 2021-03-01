#include "stdafx.h"
#include "Watchful.h"

#include "Component/PointMoveSystem.h"

Watchful::Watchful(const WatchfulDesc & desc)
	: desc(desc)
{
}

Watchful::~Watchful()
{
}

void Watchful::Call(const DelayReturn * _result)
{
	desc.Anim->Play(*desc.IdleClipNum);
	result = _result;
}

void Watchful::Update()
{
	//float lengthSq;
	//desc.Target->PreUpdate(*desc.Focus, &lengthSq);
}

void Watchful::Cancel()
{
	result = nullptr;
}
