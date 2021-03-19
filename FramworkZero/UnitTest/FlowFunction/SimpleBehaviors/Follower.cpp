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

	desc.CallAnim();
}

void Follower::Update()
{
	desc.MovingSystem->SetPoint(*desc.Focus);

	if (desc.MovingSystem->GoToPoint())
	{
		// µµÂø
		result.OnAction();
		return;
	}
}

void Follower::Cancel()
{
	result.Clear();
}
