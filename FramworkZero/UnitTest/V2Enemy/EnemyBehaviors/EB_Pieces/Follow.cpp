#include "stdafx.h"
#include "Follow.h"

#include "Component/PointMoveSystem.h"

Follow::Follow(const FollowDesc& desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&Follow::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&Follow::Update, this);
	FlowTesk::FuncCancel = bind(&Follow::Cancel, this);
}

Follow::~Follow()
{
}

void Follow::Call(const FutureAction * action)
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
