#include "stdafx.h"
#include "MeleeEnemy.h"

#include "Character/Paladin.h"
#include "Component/NormalAttack.h"

MeleeEnemy::MeleeEnemy()
{
	modelInstancing = new Paladin();

	normalAttack = new NormalAttack();
	normalAttack->InitTransform()->Position(0.0f, 90.0f, -40.0f);
	normalAttack->InitTransform()->RotationEuler(
		EulerAngle::Degree(0.0f, 0.0f, 0.0f));
	normalAttack->InitTransform()->Scale(100.0f, 180.0f, 180.0f);
	normalAttack->Tag(L"EnemyAttack");

	modelInstancing->UpdateTransforms();
}

MeleeEnemy::~MeleeEnemy()
{
	SafeDelete(normalAttack);
	SafeDelete(modelInstancing);
}

void MeleeEnemy::Update()
{
	normalAttack->Update();
	modelInstancing->Update();
	modelInstancing->UpdateTransforms();
}

void MeleeEnemy::Render()
{
	modelInstancing->Render();
}

ModelInstancing * MeleeEnemy::GetModel()
{
	return modelInstancing;
}

AttackAnimation * MeleeEnemy::MakeAttackInstance(Transform * t)
{
	return normalAttack->MakeAttackInstance(t);
}

const EnemyDesc * MeleeEnemy::GetDesc()
{
	return &desc;
}
