#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	float HP = 100.0f;
};

struct FieldEnemyClipNumGroup
{
	// 걷기 클립번호
	UINT ClipWalk = 1;
	// 둘러보기 클립번호
	UINT ClipLookAround = 0;
	// 적 발견 클립번호
	UINT ClipInSight = 3;
};

struct FieldEnemyDesc
{
	V2EnemyStatus Status;
	FieldEnemyClipNumGroup ClipGroup;
	IFocus* Player;
};

struct FieldEnemyInput
{
	FieldEnemyDesc* Desc;

	ModelInstance* Model;
	const Matrix* localWorld;

	// 순찰 경로
	const vector<Vector3>* PatrolPoints;
};