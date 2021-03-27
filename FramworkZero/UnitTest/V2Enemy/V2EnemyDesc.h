#pragma once

//#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"

struct V2EnemyStatus
{
	// ü��
	float HP = 100.0f;

	// �ȱ� �ӵ�
	float WalkSpeed = 3.0f;
	// �޸��� �ӵ�
	float RunSpeed = 10.0f;

	// �� ���� ���� (����)
	float SightRangeSq = 400.0f;
	// �� ��ħ ���� (����)
	float LoseRangeSq = 900.0f;
	// �i�ƿ��� ����
	float ApproachRangeSq = 400.0f;
	// �Ÿ� ������ ����
	float MinRange = 10.0f;
};

struct FieldEnemyClipNumGroup
{
	// ��� Ŭ����ȣ
	UINT ClipIdle = 0;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround = 0;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk = 1;
	// �޸��� Ŭ����ȣ
	UINT ClipRun = 2;
	// �� �߰� Ŭ����ȣ
	UINT ClipInSight = 3;
	// ���ݹ���
	UINT ClipHitted = 5;
	// ����
	UINT ClipDead = 6;
};

struct FieldEnemyInput
{
	V2EnemyStatus Status;
	FieldEnemyClipNumGroup ClipGroup;
	IFocus* Player;

	ModelInstance* Model;
	const Matrix* localWorld;

	// ���� ���
	const vector<Vector3>* PatrolPoints;

	class IEnemySkill* Skill;
};
