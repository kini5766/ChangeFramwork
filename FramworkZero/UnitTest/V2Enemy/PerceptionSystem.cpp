#include "stdafx.h"
#include "PerceptionSystem.h"

PerceptionSystem::PerceptionSystem(const PerceptionInput & input)
	: desc(input)
{
}

PerceptionSystem::~PerceptionSystem()
{
}

void PerceptionSystem::Update()
{
	dest = (*desc.FocusPosition) - (*desc.MinePosition);
	distanceSq = D3DXVec3LengthSq(&dest);

	if (isPerceived)
	{
		isPerceived = distanceSq <= desc.LoseRangeSq;
	}
	else
	{
		isPerceived = distanceSq <= desc.SightRangeSq;
	}
}

bool PerceptionSystem::IsPerceived()
{
	return isPerceived;
}

