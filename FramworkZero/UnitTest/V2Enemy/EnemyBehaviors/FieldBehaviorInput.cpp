#include "stdafx.h"
#include "FieldBehaviorInput.h"

#include "Component/PointMoveSystem.h"
#include "V2Enemy/PerceptionSystem.h"

FieldBehaviorDesc::FieldBehaviorDesc(const FieldBehaviorInput & input)
	: Anim(input.Anim)
	, ClipWalk(input.ClipWalk)
	, ClipRun(input.ClipRun)
	, ClipLookAround(input.ClipLookAround)
	, ClipInSight(input.ClipInSight)

	, PatrolPoints(input.PatrolPoints)
	, PatrolCount(input.PatrolCount)
	, Skill(input.Skill)
{
	MovingSystem = new PointMoveSystem(input.Trans);
}

FieldBehaviorDesc::~FieldBehaviorDesc()
{
	SafeDelete(MovingSystem);
	SafeDelete(Perceptor);
}

FieldBehaviorDesc::operator PatrollingDesc()
{
	PatrollingDesc result;

	result.Anim = Anim;
	result.ClipWalk = ClipWalk;
	result.ClipLookAround = ClipLookAround;
	result.PatrolPoints = PatrolPoints;
	result.PatrolCount = PatrolCount;

	result.MovingSystem = MovingSystem;
	result.WalkSpeed = &WalkSpeed;

	result.LookAroundTime = LookAroundTime;

	result.Perceptor = Perceptor;
	result.FuncInRange = FuncInRange;

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
	result.MovingSystem = MovingSystem;
	result.CallAnim = [=]() { 
		Anim->Play(ClipRun); 
		MovingSystem->SetMoveSpeeder(&RunSpeed); 
	};

	return result;
}

CombatPostureDesc FieldBehaviorDesc::MakeCombat()
{
	CombatPostureDesc result;
	result.StrafeAroundDesc.Anim = Anim;
	result.StrafeAroundDesc.ApproachRangeSq = ApproachRangeSq;
	result.StrafeAroundDesc.MinRange = MinRange;
	result.StrafeAroundDesc.ClipIdle = ClipIdle;
	result.StrafeAroundDesc.ClipWalk = ClipWalk;
	result.StrafeAroundDesc.ClipRun = ClipRun;
	result.StrafeAroundDesc.FuncOutRange = FuncOutRange;
	result.StrafeAroundDesc.MovingSystem = MovingSystem;
	result.StrafeAroundDesc.Perceptor = Perceptor;
	result.StrafeAroundDesc.RunSpeed = &RunSpeed;
	result.StrafeAroundDesc.WalkSpeed = &WalkSpeed;
	result.Skill = Skill;

	return result;
}
