#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	float HP = 100.0f;
};

struct V2EnemyClipNumGroup
{
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;
	// �� �߰� Ŭ����ȣ
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

	// ���� ���
	const vector<Vector3>* PatrolPoints;
};