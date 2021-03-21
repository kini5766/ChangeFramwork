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


// --
// StrafeAroundDesc
// --

PointMoverDesc StrafeAroundDesc::MakeMover()
{
	PointMoverDesc result;
	result.MovingSystem = MovingSystem;

	result.CallAnim = [=]() {
		Anim->Play(ClipRun);
		MovingSystem->SetMoveSpeeder(RunSpeed);
	};

	return result;
}

WaiterDesc StrafeAroundDesc::MakeWaiter()
{
	WaiterDesc result;
	result.Time = 1.0f;

	result.CallAnim = [=]() {
		Anim->Play(ClipIdle);
	};

	return result;
}

FollowerDesc StrafeAroundDesc::MakeFollower()
{
	FollowerDesc result;
	result.Focus = Perceptor->GetFocus();
	result.MovingSystem = MovingSystem;

	result.CallAnim = [=]() {
		Anim->Play(ClipRun);
		MovingSystem->SetMoveSpeeder(RunSpeed);
	};

	return result;
}
