#pragma once

#include "stdafx.h"


struct WaiterDesc
{
	// 대기할 시간
	float Time;
};


struct ClipPlayerDesc
{
	// 재생할 애니메이터
	Animator* Anim;
	// 재생할 클립
	UINT Clip;
};


struct PointMoverDesc
{
	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// 이동 속도
	float* WalkSpeed;
	// 목표지점의 범위 (제곱)
	float* PatrolSafeRangeSq;
	// 목표지점
	Vector3 Point;
};

struct FollowerDesc
{
	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// 달리기 속도
	float* RunSpeed;

	// 목표지점의 범위 (제곱)
	float* FollowInRangeSq;
	// 목표 지점
	Vector3* Focus;
};


