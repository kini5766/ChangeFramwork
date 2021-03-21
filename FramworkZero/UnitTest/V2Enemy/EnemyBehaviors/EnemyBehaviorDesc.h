#pragma once

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorsValues.h"

struct PatrollingDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 걷기 클립번호
	UINT ClipWalk;
	// 둘러보기 클립번호
	UINT ClipLookAround;

	// 목표지점들
	const Vector3* PatrolPoints;
	// 목표지점개수
	UINT PatrolCount;

	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// 걷기 속도
	float* WalkSpeed;
	// 둘러보기 시간
	float LookAroundTime;

	// 적 감지 시스템
	class PerceptionSystem* Perceptor;

	// 적 감지 시 호출
	function<void()> FuncInRange;


public:
	PointMoverDesc MakeMover(int pointIndex);
	WaiterDesc MakeWaiter();
};


struct StrafeAroundDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 달리기 클립번호
	UINT ClipRun;
	// 걷기 클립번호
	UINT ClipWalk;
	// 대기 클립번호
	UINT ClipIdle;

	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// 걷기 속도
	float* WalkSpeed;
	// 달리기 속도
	float* RunSpeed;

	// 적 감지 시스템
	class PerceptionSystem* Perceptor;

	// 거리 유지 범위
	float MinRangeSq;
	// 쫒아오기 범위
	float ApproachRangeSq;

	// 적 놓치면 호출
	function<void()> FuncOutRange;

public:
	PointMoverDesc MakeMover();
	WaiterDesc MakeWaiter();
	FollowerDesc MakeFollower();
};


struct CombatPostureDesc
{
	// 스킬 사용 아닐 때 행동
	StrafeAroundDesc StrafeAroundDesc;

	// 적 스킬
	class IEnemySkill* Skill;
};


