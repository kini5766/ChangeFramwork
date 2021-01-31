#include "stdafx.h"
#include "EnemyInstancingDesc.h"

EnemyInstancingDesc::EnemyInstancingDesc()
{
	TImGui = new ComponentTransform();
	ModelTemp = new Transform();
	ModelTemp->Position(0.0f, 90.0f, 0.0f);
	ModelTemp->Scale(75.0f, 180.0f, 75.0f);
}

EnemyInstancingDesc::~EnemyInstancingDesc()
{
	for (auto d : Units)
		SafeDelete(d);
	
	SafeDelete(ModelTemp);
	SafeDelete(TImGui);
}


void EnemyInstancingDesc::AddInstance()
{
	EnemyInstanceDesc* unit = new EnemyInstanceDesc();
	SetScale(&unit->Transform);
	unit->PatrolPoints.push_back(Vector3(0, 0, 0));
	unit->PatrolPoints.push_back(Vector3(0, 0, 1));
	Units.push_back(unit);
}

void EnemyInstancingDesc::SetScale(Transform * t)
{
	switch (EnemyType)
	{
	case 0: t->Scale(0.03f, 0.03f, 0.03f); break;
	case 1: t->Scale(0.025f, 0.025f, 0.025f); break;
	}
}

void EnemyInstancingDesc::Render()
{
	for (UINT i = 0; i < Units.size(); i++)
	{
		Matrix local;
		ModelTemp->LocalWorld(&local);
		ModelTemp->SetParent(&Units[i]->Transform);
		ModelTemp->LocalWorld(local);
		Debug::Box->RenderBox(ModelTemp, BoxColor);
	}
}
