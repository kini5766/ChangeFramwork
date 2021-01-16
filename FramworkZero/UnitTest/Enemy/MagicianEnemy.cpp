#include "stdafx.h"
#include "MagicianEnemy.h"

#include "Tools/Viewer/IFocus.h"
#include "Component/MagicAttack.h"

#include "Character/Friedrich.h"

MagicianEnemy::MagicianEnemy(Shader * shader, IFocus* player)
{
	modelInstancing = new ModelSkinnedInstancing(shader, {
		/*매쉬*/ L"Friedrich/Mesh",
		/*매터리얼*/ L"Friedrich/Mesh",
		/*클립*/ {
			L"Friedrich/Idle",  // 0
			L"Friedrich/Walk",  // 1
			L"Friedrich/Run",  // 2
			L"Friedrich/Taunt",  // 3
			L"Friedrich/Attack",  // 4
			L"Friedrich/React",  // 5
			L"Friedrich/Fall",  // 6
		}
		});

	sphere = new MeshInstancing(shader, new MeshSphere(0.5f));
	magicAttack = new MagicAttack(player, sphere);
	magicAttack->InitTransform()->Position(-25.0f, 135.0f, -40.0f);
	magicAttack->InitTransform()->Rotation(EulerAngle(180.0f, 0.0f, 0.0f));
	magicAttack->Tag(L"EnemyAttack");

	desc.HP = 80.0f;
	desc.AttackRange = 25.0f;
	desc.DetectionRange = 40.0f;
	//desc.TurnSpeed = 10.0f;

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
	Friedrich::BindAnimation(animator, model);
}

AttackAnimation * MagicianEnemy::MakeAttackInstance(Transform * t)
{
	return magicAttack->MakeAttackInstance(t);
}

const EnemyDesc * MagicianEnemy::GetDesc()
{
	return &desc;
}
