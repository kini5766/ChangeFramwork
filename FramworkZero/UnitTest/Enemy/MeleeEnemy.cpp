#include "stdafx.h"
#include "MeleeEnemy.h"

#include "Character/Paladin.h"
#include "Component/NormalAttack.h"

MeleeEnemy::MeleeEnemy()
{
	modelInstancing = new ModelInstancing({
			/*매쉬*/ L"Paladin/Mesh",
			/*매터리얼*/ L"Paladin/Mesh",
			/*클립*/ {
				L"Paladin/Idle",  // 0
				L"Paladin/Walk",  // 1
				L"Paladin/Run",  // 2
				L"Paladin/Taunt",  // 3
				L"Paladin/Attack",  // 4
				L"Paladin/React",  // 5
				L"Paladin/Fall",  // 6
			}
		});

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

void MeleeEnemy::BindAnimation(Animator * animator, AnimationAdapter * model)
{
	Paladin::BindAnimation(animator, model);
}

AttackAnimation * MeleeEnemy::MakeAttackInstance(Transform * t)
{
	return normalAttack->MakeAttackInstance(t);
}

const EnemyDesc * MeleeEnemy::GetDesc()
{
	return &desc;
}
