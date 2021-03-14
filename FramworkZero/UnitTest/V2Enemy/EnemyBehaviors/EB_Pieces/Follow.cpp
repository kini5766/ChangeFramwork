#include "stdafx.h"
#include "Follow.h"

#include "Component/PointMoveSystem.h"

Follow::Follow(const FollowDesc& desc)
	: desc(desc)
{
}

Follow::~Follow()
{
}

void Follow::Call(const ReturnAction * action)
{
	desc.Animator->Play(*desc.ClipRun);
	result.SetAction(action);
}

void Follow::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(*desc.Focus, &lengthSq);

	if (lengthSq < (*desc.FollowInRangeSq))
	{
		// 주변에 도착
		result.OnAction();
		return;
	}

	desc.Target->SetMoveSpeed(*desc.RunSpeed);
	if (desc.Target->Update())
	{
		// 정확히 도착
		result.OnAction();
		return;
	}
}

void Follow::Cancel()
{
	result.Clear();
}
