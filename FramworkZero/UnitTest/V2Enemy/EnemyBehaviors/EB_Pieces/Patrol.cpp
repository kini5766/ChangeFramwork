#include "stdafx.h"
#include "Patrol.h"

#include "Component/PointMoveSystem.h"

Patrol::Patrol(const PatrolDesc & desc)
	: desc(desc)
{
}

Patrol::~Patrol()
{
}

void Patrol::Call(const ReturnAction * action)
{
	float lengthSq;
	desc.Target->PreUpdate(desc.Point, &lengthSq);
	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// 주변에 도착
		result.Clear();
		(*action)();
		return;
	}
	else
	{
		desc.Anim->Play(*desc.ClipWalk);
		result.SetAction(action);
	}
}

void Patrol::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(desc.Point, &lengthSq);

	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// 주변에 도착
		result.OnAction();
		return;
	}

	desc.Target->SetMoveSpeed(*desc.WalkSpeed);
	if (desc.Target->Update())
	{
		// 정확히 도착
		result.OnAction();
		return;
	}
}

void Patrol::Cancel()
{
	result.Clear();
}
