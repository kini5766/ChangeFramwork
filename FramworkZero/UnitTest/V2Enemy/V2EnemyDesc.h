#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	float HP = 100.0f;
};

struct FieldEnemyClipNumGroup
{
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk = 1;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround = 0;
	// �� �߰� Ŭ����ȣ
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

	// ���� ���
	const vector<Vector3>* PatrolPoints;
};