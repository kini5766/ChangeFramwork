#pragma once

#include "FlowFunction/FlowFunction.h"


struct PlaingOnceDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 재생할 클립
	UINT Clip;
};

// --
// Patrolling
// --
struct MovingToPointDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 걷기 클립번호
	UINT* ClipWalk;

	// 이동 시스템
	class PointMoveSystem* Target;
	// 이동 속도
	float* WalkSpeed;
	// 목표지점의 범위 (제곱)
	float* PatrolSafeRangeSq;
	// 목표지점
	Vector3 Point;
};
struct LookAroundDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 둘러보기 클립
	const UINT* ClipLookAround;

	// 둘러보기 시간
	float* LookAroundTime;
};


// --
// InBattle
// --
struct FollowDesc
{
	// 이동 시스템
	class PointMoveSystem* Target;
	// 달리기 속도
	float* RunSpeed;

	// 애니메이터 대상
	Animator* Animator;
	// 달리기 클립번호
	UINT* ClipRun;

	// 목표지점의 범위 (제곱)
	float* FollowInRangeSq;
	// 목표 지점
	Vector3* Focus;
};
struct WatchfulDesc
{
	// 이동 시스템
	//class PointMoveSystem* Target;
	// 와리가리 속도
	//float* WalkSpeed;

	// 애니메이터 대상
	Animator* Anim;
	// 걷기 클립번호
	//UINT* WalkClipNum;
	// 대기 클립번호
	UINT* IdleClipNum;

	// 최소 적과의 거리
	//float* InnerRange;
	// 최대 적과의 거리
	//float* OuterRange;
	// 목표 지점
	//Vector3* Focus;
};