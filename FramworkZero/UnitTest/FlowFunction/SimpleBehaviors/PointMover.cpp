#include "stdafx.h"
#include "PointMover.h"

#include "Component/PointMoveSystem.h"

PointMover::PointMover(const PointMoverDesc & desc)
	: desc(desc)
{
}

PointMover::~PointMover()
{
}

void PointMover::Call(const FutureAction * action)
{
	desc.MovingSystem->SetPoint(desc.Point);
	result.SetAction(action);
}

void PointMover::Update()
{
	if (desc.MovingSystem->GoToPoint())
	{
		// µµÂø
		result.OnAction();
		return;
	}
}

void PointMover::Cancel()
{
	result.Clear();
}
