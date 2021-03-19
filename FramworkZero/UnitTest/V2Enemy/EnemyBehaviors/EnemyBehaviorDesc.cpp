#include "stdafx.h"
#include "EnemyBehaviorDesc.h"

#include "Component/PointMoveSystem.h"

PointMoverDesc PatrollingDesc::MakeMover(int pointIndex)
{
	PointMoverDesc result;
	result.MovingSystem = MovingSystem;
	result.Point = PatrolPoints[pointIndex];

	result.CallAnim = [=]() {
		Anim->Play(ClipWalk);
		MovingSystem->SetMoveSpeeder(WalkSpeed);
	};

	return result;
}

WaiterDesc PatrollingDesc::MakeWaiter()
{
	WaiterDesc result;
	result.Time = LookAroundTime;

	result.CallAnim = [=]() { 
		Anim->Play(ClipLookAround); 
	};

	return result;
}
