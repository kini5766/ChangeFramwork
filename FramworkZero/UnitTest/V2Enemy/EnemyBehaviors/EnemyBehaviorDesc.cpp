#include "stdafx.h"
#include "EnemyBehaviorDesc.h"

MovingToPointDesc PatrollingDesc::CastPatrol(int pointIndex)
{
	MovingToPointDesc result;

	result.Target = Target;
	result.WalkSpeed = WalkSpeed;
	result.Anim = Anim;
	result.ClipWalk = ClipWalk;
	result.PatrolSafeRangeSq = PatrolSafeRangeSq;
	result.Point = PatrolPoints[pointIndex];

	return result;
}

PatrollingDesc::operator LookAroundDesc()
{
	LookAroundDesc result;

	result.Anim = Anim;
	result.ClipLookAround = ClipLookAround;
	result.LookAroundTime = LookAroundTime;

	return result;
}
