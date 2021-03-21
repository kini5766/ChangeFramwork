#pragma once

struct FieldBehaviorInput
{
	Transform* Trans;

	// 재생할 애니메이터
	Animator* Anim;
	// 대기 클립번호
	UINT ClipIdle;
	// 걷기 클립번호
	UINT ClipWalk;
	// 달리기 클립번호 todo
	UINT ClipRun;
	// 둘러보기 클립번호
	UINT ClipLookAround;
	// 적 발견 클립번호
	UINT ClipInSight;


	// 목표지점들
	const Vector3* PatrolPoints;
	// 목표지점개수
	UINT PatrolCount;

	// todo
	class IEnemySkill* Skill;
};



#include "EnemyBehaviorDesc.h"
#include "Rendering/Camera/Main/IFocus.h"

struct FieldBehaviorDesc
{
public:
	FieldBehaviorDesc(const FieldBehaviorInput& input);
	~FieldBehaviorDesc();

public:
	// 재생할 애니메이터
	Animator* Anim;
	// 대기 클립번호
	UINT ClipIdle;
	// 걷기 클립번호
	UINT ClipWalk;
	// 달리기 클립번호
	UINT ClipRun;
	// 둘러보기 클립번호
	UINT ClipLookAround;
	// 적 발견 클립번호
	UINT ClipInSight;

	// 목표지점들
	const Vector3* PatrolPoints;
	// 목표지점개수
	UINT PatrolCount;

	// 적 스킬
	class IEnemySkill* Skill;


	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// [직접세팅] 걷기 속도
	float WalkSpeed = 3.0f;
	// [직접세팅] 달리기 속도
	float RunSpeed = 10.0f;

	// [직접세팅] 적 감지 시스템
	class PerceptionSystem* Perceptor = nullptr;
	// [직접세팅] 적 감지 범위 (제곱)
	float SightRangeSq = 400.0f;
	// [직접세팅] 적 놓침 범위 (제곱)
	float LoseRangeSq = 900.0f;
	// [직접세팅] 쫒아오기 범위
	float ApproachRangeSq = 400.0f;
	// [직접세팅] 거리 유지할 범위
	float MinRange = 10.0f;

	// [직접세팅] 둘러보기 시간
	float LookAroundTime = 1.0f;

	// [직접세팅] 감지 안에 들어왔을 때
	function<void()> FuncInRange;
	// [직접세팅] 감지 밖에 들어왔을 때
	function<void()> FuncOutRange;


public:
	operator PatrollingDesc();

	ClipPlayerDesc MakeInSight();
	PointMoverDesc MakeComeback();
	CombatPostureDesc MakeCombat();
};
