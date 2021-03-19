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
	desc.MovingSystem->SetPoint(desc.Point);
	result.SetAction(action);

	desc.CallAnim();
}

void PointMover::Update()
{
	if (desc.MovingSystem->GoToPoint())
	{
		// ����
		result.OnAction();
		return;
	}
}

void PointMover::Cancel()
{
	result.Clear();
}
