#include "stdafx.h"
#include "FieldBehaviorInput.h"

#include "Component/PointMoveSystem.h"
#include "V2Enemy/EnemyDetectionSystem.h"

FieldBehaviorDesc::FieldBehaviorDesc(const FieldBehaviorInput & input)
	: Anim(input.Anim)
	, ClipWalk(input.ClipWalk)
	, ClipLookAround(input.ClipLookAround)
	, ClipInSight(input.ClipInSight)

	, PatrolPoints(input.PatrolPoints)
	, PatrolCount(input.PatrolCount)
{
	MoveSystem = new PointMoveSystem(input.Trans);
	//DetectionSystem = new EnemyDetectionSystem();
}

FieldBehaviorDesc::~FieldBehaviorDesc()
{
	SafeDelete(MoveSystem);
	SafeDelete(DetectionSystem);
}

FieldBehaviorDesc::operator PatrollingDesc()
{
	PatrollingDesc result;

	result.Anim = Anim;
	result.ClipWalk = ClipWalk;
	result.ClipLookAround = ClipLookAround;
	result.PatrolPoints = PatrolPoints;
	result.PatrolCount = PatrolCount;

	result.MovingSystem = MoveSystem;
	result.WalkSpeed = &WalkSpeed;

	result.DetectionSystem = DetectionSystem;

	result.LookAroundTime = LookAroundTime;

	return result;
}

ClipPlayerDesc FieldBehaviorDesc::MakeInSight()
{
	ClipPlayerDesc result;
	result.Anim = Anim;
	result.Clip = ClipInSight;

	return result;
}

PointMoverDesc FieldBehaviorDesc::MakeComeback()
{
	PointMoverDesc result;
	result.MovingSystem = MoveSystem;
	//result.CallAnim = [=]() { 
	//desc.Anim->Play(desc.ClipRun); 
	//MovingSystem->SetMoveSpeeder(RunSpeed); 
	//};

	return result;
}
