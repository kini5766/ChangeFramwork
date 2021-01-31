#include "stdafx.h"
#include "EnemyInstancingSpawner.h"

#include "EditorDemo/Component/TransformLoader.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Enemy/EnemyInstancing.h"
#include "Enemy/MeleeEnemy.h"
#include "Enemy/MagicianEnemy.h"

EnemyInstancingSpawner::EnemyInstancingSpawner(SceneValue* scene)
{
	player = (IFocus*)scene->GetValue("IFocusPlayer");
}

EnemyInstancingSpawner::~EnemyInstancingSpawner()
{
	SafeDelete(enemy);
}

void EnemyInstancingSpawner::Update()
{
	enemy->Update();
}

void EnemyInstancingSpawner::Render()
{
	enemy->Render();
}


#pragma region IObjectSpawner

void EnemyInstancingSpawner::Initialize(SceneValueUnit * scene)
{
	scene->Updater->Action = bind(&EnemyInstancingSpawner::Update, this);
	scene->Updater->bDestroy = false;

	scene->Renderer->Action = bind(&EnemyInstancingSpawner::Render, this);
	scene->Renderer->bDestroy = false;
}

bool EnemyInstancingSpawner::Load(BinaryReader * r)
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

void EnemyInstancingSpawner::CreateEnemy(int item)
{
	switch (item)
	{
	case 0: enemy = new EnemyInstancing(player, unique_ptr<IEnemy>(new MeleeEnemy())); break;
	case 1: enemy = new EnemyInstancing(player, unique_ptr<IEnemy>(new MagicianEnemy(player))); break;
	}
}

void EnemyInstancingSpawner::SetScale(Transform * t)
{
	switch (enemyType)
	{
	case 0: t->Scale(0.03f, 0.03f, 0.03f); break;
	case 1: t->Scale(0.025f, 0.025f, 0.025f); break;
	}
}

#pragma endregion
