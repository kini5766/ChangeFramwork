#include "stdafx.h"
#include "Follower.h"

#include "Component/PointMoveSystem.h"

Follower::Follower(const FollowerDesc& desc)
	: desc(desc)
{
}

Follower::~Follower()
{
}

void Follower::Call(const FutureAction * action)
{
	result.SetAction(action);
}

void Follower::Update()
{
	desc.MovingSystem->SetPoint(desc.FuncGetFocus());
	desc.MovingSystem->GoToPoint();
}

void Follower::Hold()
{
	result.Clear();
}
