#include "stdafx.h"
#include "PointMover.h"

#include "Component/PointMoveSystem.h"

PointMover::PointMover(const PointMoverDesc & desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&PointMover::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&PointMover::Update, this);
	FlowTesk::FuncCancel = bind(&PointMover::Cancel, this);
}

PointMover::~PointMover()
{
}

void PointMover::Call(const FutureAction * action)
{
	result.SetAction(action);
}

void PointMover::Update()
{
	float lengthSq;
	desc.MovingSystem->PreUpdate(desc.Point, &lengthSq);
	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// 주변에 도착
		result.OnAction();
		return;
	}

	desc.MovingSystem->SetMoveSpeed(*desc.WalkSpeed);
	if (desc.MovingSystem->Update())
	{
		// 정확히 도착
		result.OnAction();
		return;
	}
}

void PointMover::Cancel()
{
	result.Clear();
}
