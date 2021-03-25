#include "stdafx.h"
#include "EnemyBehaviorDesc.h"

#include "Component/PointMoveSystem.h"
#include "V2Enemy/PerceptionSystem.h"


// --
// PatrollingDesc
// --

PointMoverDesc PatrollingDesc::MakeMover(int pointIndex)
{
	PointMoverDesc result;
	result.MovingSystem = MovingSystem;
	result.Point = PatrolPoints[pointIndex];

	return result;
}

WaiterDesc PatrollingDesc::MakeWaiter()
{
	WaiterDesc result;
	result.Time = LookAroundTime;

	return result;
}


// --
// StrafeAroundDesc
// --

PointMoverDesc StrafeAroundDesc::MakeMover()
{
	PointMoverDesc result;
	result.MovingSystem = MovingSystem;

	return result;
}

WaiterDesc StrafeAroundDesc::MakeWaiter()
{
	WaiterDesc result;
	result.Time = 1.0f;

	return result;
}

FollowerDesc StrafeAroundDesc::MakeFollower()
{
	FollowerDesc result;
	result.Focus = Perceptor->GetFocus();
	result.MovingSystem = MovingSystem;

	return result;
}
