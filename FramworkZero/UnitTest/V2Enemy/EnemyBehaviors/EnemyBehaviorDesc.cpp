#include "stdafx.h"
#include "EnemyBehaviorDesc.h"

ClipPlayerDesc PatrollingDesc::MakeWalk()
{
	ClipPlayerDesc result;
	result.Anim = Anim;
	result.Clip = ClipWalk;

	return result;
}

PointMoverDesc PatrollingDesc::MakeMover(int pointIndex)
{
	PointMoverDesc result;
	result.MovingSystem = MovingSystem;
	result.WalkSpeed = WalkSpeed;
	result.PatrolSafeRangeSq = PatrolSafeRangeSq;
	result.Point = PatrolPoints[pointIndex];

	return result;
}

ClipPlayerDesc PatrollingDesc::MakeLookAround()
{
	ClipPlayerDesc result;
	result.Anim = Anim;
	result.Clip = ClipLookAround;

	return result;
}

WaiterDesc PatrollingDesc::MakeWaiter()
{
	WaiterDesc result;
	result.Time = LookAroundTime;

	return result;
}
