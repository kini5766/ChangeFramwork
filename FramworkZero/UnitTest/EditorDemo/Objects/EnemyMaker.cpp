#include "stdafx.h"
#include "EnemyMaker.h"


// --
// EnemyMakerMagic
// --

#include "V2Enemy/V2EnemyMelee.h"
EnemyMakerMelee::EnemyMakerMelee(IFocus* player)
{
	enemy = new V2EnemyMelee(player);
}

EnemyMakerMelee::~EnemyMakerMelee()
{
	SafeDelete(enemy);
}

void EnemyMakerMelee::Update()
{
	enemy->Update();
}

void EnemyMakerMelee::Render()
{
	enemy->Render();
}

void EnemyMakerMelee::PostRender()
{
	enemy->PostRender();
}

void EnemyMakerMelee::AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints)
{
	enemy->AddInstance(localWorld, patrolPoints);
}


// --
// EnemyMakerMagic
// --

#include "V2Enemy/V2EnemyMagic.h"
EnemyMakerMagic::EnemyMakerMagic(IFocus* player)
{
	enemy = new V2EnemyMagic(player);
}

EnemyMakerMagic::~EnemyMakerMagic()
{
	SafeDelete(enemy);
}

void EnemyMakerMagic::Update()
{
	enemy->Update();
}

void EnemyMakerMagic::Render()
{
	enemy->Render();
}

void EnemyMakerMagic::PostRender()
{
	enemy->PostRender();
}

void EnemyMakerMagic::AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints)
{
	enemy->AddInstance(localWorld, patrolPoints);
}
