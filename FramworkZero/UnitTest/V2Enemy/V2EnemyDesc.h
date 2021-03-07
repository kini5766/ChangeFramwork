#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	float HP = 100.0f;
};

struct V2EnemyClipNumGroup
{
	// 걷기 클립번호
	UINT ClipWalk;
	// 둘러보기 클립번호
	UINT ClipLookAround;
	// 적 발견 클립번호
	UINT ClipInSight;
};


struct V2EnemyInput
{
	V2EnemyStatus Status;

	IFocus* Player;
	V2EnemyClipNumGroup ClipGroup;
};


struct V2EnemyInstanceDesc
{
	const V2EnemyInput* Parent;
	ModelInstance* Model;

	const Matrix* localWorld;

	// 순찰 경로
	const vector<Vector3>* PatrolPoints;
};