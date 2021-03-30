#include "stdafx.h"
#include "V2EnemyInstancingSpawner.h"

#include "EditorDemo/Component/TransformLoader.h"

#include "Rendering/Camera/Main/OrbitCamera.h"
#include "V2Enemy/V2EnemyMelee.h"
#include "V2Enemy/V2EnemyMagic.h"
#include "EnemyMaker.h"

V2EnemyInstancingSpawner::V2EnemyInstancingSpawner(SceneValue* scene)
{
	player = (IFocus*)scene->GetValue("IFocusPlayer");
}

V2EnemyInstancingSpawner::~V2EnemyInstancingSpawner()
{
	SafeDelete(enemy);
}

void V2EnemyInstancingSpawner::Update()
{
	enemy->Update();
}

void V2EnemyInstancingSpawner::Render()
{
	enemy->Render();
}

void V2EnemyInstancingSpawner::PostRender()
{
	enemy->PostRender();
}


#pragma region IObjectSpawner

void V2EnemyInstancingSpawner::Initialize(SceneValueUnit * scene)
{
	scene->Updater->Action = bind(&V2EnemyInstancingSpawner::Update, this);
	scene->Updater->bDestroy = false;

	scene->Renderer->Action = bind(&V2EnemyInstancingSpawner::Render, this);
	scene->Renderer->bDestroy = false;

	scene->PostRenderer->Action = bind(&V2EnemyInstancingSpawner::PostRender, this);
	scene->PostRenderer->bDestroy = false;
}

bool V2EnemyInstancingSpawner::Load(BinaryReader * r)
{
	enemyType = r->Int();
	CreateEnemy(enemyType);

	UINT size = r->UInt();
	if (size == 0)
		return false;

	TransformLoader tLoader;

	for (UINT i = 0; i < size; i++)
	{
		Transform t;
		tLoader.Load(&t, r);
		SetScale(&t);

		Matrix w;
		t.LocalWorld(&w);

		vector<Vector3> patrolPoints;
		patrolPoints.push_back(r->Vector3());
		patrolPoints.push_back(r->Vector3());
		enemy->AddInstance(w, &patrolPoints);
	}

	return true;
}

void V2EnemyInstancingSpawner::CreateEnemy(int item)
{
	switch (item)
	{
	case 0: enemy = new EnemyMakerMelee(player); break;
	case 1: enemy = new EnemyMakerMagic(player); break;
	}
}

void V2EnemyInstancingSpawner::SetScale(Transform * t)
{
	switch (enemyType)
	{
	case 0: t->Scale(0.03f, 0.03f, 0.03f); break;
	case 1: t->Scale(0.025f, 0.025f, 0.025f); break;
	}
}

#pragma endregion
