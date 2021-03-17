#include "stdafx.h"
#include "Follower.h"

#include "Component/PointMoveSystem.h"

Follower::Follower(const FollowerDesc& desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&Follower::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&Follower::Update, this);
	FlowTesk::FuncCancel = bind(&Follower::Cancel, this);
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
	float lengthSq;
	desc.MovingSystem->PreUpdate(*desc.Focus, &lengthSq);

	if (lengthSq < (*desc.FollowInRangeSq))
	{
		// 주변에 도착
		result.OnAction();
		return;
	}

	desc.MovingSystem->SetMoveSpeed(*desc.RunSpeed);
	if (desc.MovingSystem->Update())
	{
		// 정확히 도착
		result.OnAction();
		return;
	}
}

void Follower::Cancel()
{
	result.Clear();
}
