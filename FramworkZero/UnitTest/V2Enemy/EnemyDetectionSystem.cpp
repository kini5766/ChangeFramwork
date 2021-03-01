#include "stdafx.h"
#include "EnemyDetectionSystem.h"

EnemyDetectionSystem::EnemyDetectionSystem(const EnemyDetectionInput & input)
	: desc(input)
{
}

EnemyDetectionSystem::~EnemyDetectionSystem()
{
}

bool EnemyDetectionSystem::IsInRange()
{
	Vector3 dest = (*desc.FocusPosition) - (*desc.MinePosition);
	float distanceSq = D3DXVec3LengthSq(&dest);

	return distanceSq <= desc.DetectionRangeSq;
}

void EnemyDetectionSystem::OnInRange()
{
	desc.FuncInRange();
}
