#include "stdafx.h"
#include "MovingToPoint.h"

#include "Component/PointMoveSystem.h"

MovingToPoint::MovingToPoint(const MovingToPointDesc & desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&MovingToPoint::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&MovingToPoint::Update, this);
	FlowTesk::FuncCancel = bind(&MovingToPoint::Cancel, this);
}

MovingToPoint::~MovingToPoint()
{
}

void MovingToPoint::Call(const FutureAction * action)
{
	float lengthSq;
	desc.Target->PreUpdate(desc.Point, &lengthSq);
	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// �ֺ��� ����
		result.Clear();
		(*action)();
		return;
	}
	

	desc.Anim->Play(*desc.ClipWalk);
	result.SetAction(action);
}

void MovingToPoint::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(desc.Point, &lengthSq);
	if (lengthSq < (*desc.PatrolSafeRangeSq))
	{
		// �ֺ��� ����
		result.OnAction();
		return;
	}

	desc.Target->SetMoveSpeed(*desc.WalkSpeed);
	if (desc.Target->Update())
	{
		// ��Ȯ�� ����
		result.OnAction();
		return;
	}
}

void MovingToPoint::Cancel()
{
	result.Clear();
}
