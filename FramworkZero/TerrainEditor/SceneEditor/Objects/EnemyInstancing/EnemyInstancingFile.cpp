#include "stdafx.h"
#include "EnemyInstancingFile.h"

#include "Utilities/BinaryFile.h"
#include "EnemyInstancingDesc.h"

EnemyInstancingFile::EnemyInstancingFile(EnemyInstancingDesc * desc)
	: desc(desc)
{
}

EnemyInstancingFile::~EnemyInstancingFile()
{
}

void EnemyInstancingFile::Save(BinaryWriter * w)
{
	w->Int(desc->EnemyType);

	UINT size = desc->Units.size();
	w->UInt(size);
	for (EnemyInstanceDesc* unit : desc->Units)
	{
		desc->TImGui->Save(&unit->Transform, w);
		w->Vector3(unit->PatrolPoints[0]);
		w->Vector3(unit->PatrolPoints[1]);
	}
}

void EnemyInstancingFile::Load(BinaryReader * r)
{
	desc->EnemyType = r->Int();

	UINT size = desc->Units.size();
	size = r->UInt();
	for (UINT i = 0; i < size; i++)
	{
		EnemyInstanceDesc* unit = new EnemyInstanceDesc();

		desc->TImGui->Load(&unit->Transform, r);
		unit->PatrolPoints.push_back(r->Vector3());
		unit->PatrolPoints.push_back(r->Vector3());

		desc->Units.push_back(unit);
	}
}
