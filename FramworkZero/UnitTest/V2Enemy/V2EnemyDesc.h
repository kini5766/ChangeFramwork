#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	// 체력
	float HP = 100.0f;

	// 걷기 속도
	float WalkSpeed = 3.0f;
	// 달리기 속도
	float RunSpeed = 10.0f;

	// 적 감지 범위 (제곱)
	float SightRangeSq = 400.0f;
	// 적 놓침 범위 (제곱)
	float LoseRangeSq = 900.0f;
	// 쫒아오기 범위
	float ApproachRangeSq = 400.0f;
	// 거리 유지할 범위
	float MinRange = 10.0f;
};

struct FieldEnemyClipNumGroup
{
	// 대기 클립번호
	UINT ClipIdle = 0;
	// 둘러보기 클립번호
	UINT ClipLookAround = 0;
	// 걷기 클립번호
	UINT ClipWalk = 1;
	// 달리기 클립번호
	UINT ClipRun = 2;
	// 적 발견 클립번호
	UINT ClipInSight = 3;
	// 공격받음
	UINT ClipHitted = 5;
	// 죽음
	UINT ClipDead = 6;
};

struct FieldEnemyInput
{
	V2EnemyStatus Status;
	FieldEnemyClipNumGroup ClipGroup;
	IFocus* Player;

	ModelInstance* Model;
	const Matrix* localWorld;

	// 순찰 경로
	const vector<Vector3>* PatrolPoints;

	class IEnemySkill* Skill;
};
