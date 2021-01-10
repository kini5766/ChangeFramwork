#include "stdafx.h"
#include "MagicianEnemy.h"

#include "Tools/Viewer/IFocus.h"
#include "Character/Paladin.h"
#include "Component/MagicAttack.h"

MagicianEnemy::MagicianEnemy(Shader * shader, IFocus* player)
{
	modelInstancing = new ModelSkinnedInstancing(shader, {
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

	sphere = new MeshInstancing(shader, new MeshSphere(0.5f));
	magicAttack = new MagicAttack(player, sphere);
	magicAttack->InitTransform()->Position(0.0f, 90.0f, -40.0f);
	magicAttack->InitTransform()->Rotation(0.0f, 0.0f, 0.0f);
	magicAttack->Tag(L"EnemyAttack");

	desc.HP = 80.0f;
	desc.AttackRange = 25.0f;
	desc.DetectionRange = 40.0f;

	modelInstancing->UpdateTransforms();
	modelInstancing->UpdateColors();
	modelInstancing->Pass(1);
}

MagicianEnemy::~MagicianEnemy()
{
	SafeDelete(magicAttack);
	SafeDelete(sphere);
	SafeDelete(modelInstancing);
}

void MagicianEnemy::Update()
{
	magicAttack->Update();
	modelInstancing->Update();
	modelInstancing->UpdateTransforms();

	sphere->Update();
	sphere->UpdateTransforms();
}

void MagicianEnemy::Render()
{
	modelInstancing->Render();
	sphere->Render();
}

ModelSkinnedInstancing * MagicianEnemy::GetModel()
{
	return modelInstancing;
}

void MagicianEnemy::BindAnimation(Animator * animator, ModelAnimation * model)
{
	Paladin::BindAnimation(animator, model);
}

AttackAnimation * MagicianEnemy::MakeAttackInstance(Transform * t)
{
	return magicAttack->MakeAttackInstance(t);
}

const EnemyDesc * MagicianEnemy::GetDesc()
{
	return &desc;
}
