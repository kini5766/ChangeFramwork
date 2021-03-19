#pragma once

#include "stdafx.h"


struct WaiterDesc
{
	// 대기할 시간
	float Time;

	// 이 함수가 작동 시 애니메이션 재생
	function<void()> CallAnim = []() {};
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
	// 목표지점
	Vector3 Point;

	// 이 함수가 작동 시 애니메이션 재생
	function<void()> CallAnim = []() {};
};

struct FollowerDesc
{
	// 이동 시스템
	class PointMoveSystem* MovingSystem;
	// 이동 속도
	float* MoveSpeed;

	// 목표 지점
	Vector3* Focus;

	// 이 함수가 작동 시 애니메이션 재생
	function<void()> CallAnim = []() {};
};


