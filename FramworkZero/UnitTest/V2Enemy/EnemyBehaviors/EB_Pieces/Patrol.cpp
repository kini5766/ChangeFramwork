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

void Patrol::Call(const DelayReturn * _result)
{
	desc.Anim->Play(*desc.ClipWalk);
	result = _result;
}

void Patrol::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(desc.Point, &lengthSq);

	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// 주변에 도착
		(*result)();
		result = nullptr;
		return;
	}

	desc.Target->SetMoveSpeed(*desc.WalkSpeed);
	if (desc.Target->Update())
	{
		// 정확히 도착
		(*result)();
		result = nullptr;
		return;
	}
}

void Patrol::Cancel()
{
	result = nullptr;
}
