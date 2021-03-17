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
	// 목표지점의 범위 (제곱)
	float* PatrolSafeRangeSq;
	// 이동 속도
	float* WalkSpeed;
	// 둘러보기 시간
	float LookAroundTime;

	// 적 감지 시스템
	class EnemyDetectionSystem* DetectionSystem;

public:
	ClipPlayerDesc MakeWalk();
	PointMoverDesc MakeMover(int pointIndex);
	ClipPlayerDesc MakeLookAround();
	WaiterDesc MakeWaiter();
};
