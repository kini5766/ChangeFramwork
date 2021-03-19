#pragma once

struct FieldBehaviorInput
{
	Transform* Trans;

	// 재생할 애니메이터
	Animator* Anim;
	// 걷기 클립번호
	UINT ClipWalk;
	// 둘러보기 클립번호
	UINT ClipLookAround;
	// 적 발견 클립번호
	UINT ClipInSight;


	// 목표지점들
	const Vector3* PatrolPoints;
	// 목표지점개수
	UINT PatrolCount;
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
	// 걷기 클립번호
	UINT ClipWalk;
	// 둘러보기 클립번호
	UINT ClipLookAround;
	// 적 발견 클립번호
	UINT ClipInSight;

	// 목표지점들
	const Vector3* PatrolPoints;
	// 목표지점개수
	UINT PatrolCount;

	// 이동 시스템
	class PointMoveSystem* MoveSystem; 
	// [직접세팅] 걷기 속도
	float WalkSpeed = 3.0f;

	// [직접세팅] 적 감지 시스템
	class EnemyDetectionSystem* DetectionSystem = nullptr;

	// [직접세팅] 둘러보기 시간
	float LookAroundTime = 1.0f;

public:
	operator PatrollingDesc();

	ClipPlayerDesc MakeInSight();
};
