#include "stdafx.h"
#include "EnemyInstancing.h"

#include "Character/Kachujin.h"
#include "Component/HPSystem.h"
#include "EnemyAttackSystem.h"

EnemyInstancing::EnemyInstancing(Shader * shader, IFocus* player)
{
	modelInstancing = new ModelSkinnedInstancing(shader, {
			/*매쉬*/ L"Kachujin/Mesh",
			/*매터리얼*/ L"Kachujin/Mesh",
			/*클립*/ {
				L"Kachujin/Idle",
				L"Kachujin/Walk",
				L"Kachujin/Run",
				L"Kachujin/Slash",
				L"Kachujin/Uprock",
			}
		});

	ModelSkinnedInstance* instance = modelInstancing->AddInstance();
	instance->GetTransform()->Position(25.0f, 0.0f, 25.0f);
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	instance->GetTransform()->RotationDegree(0.0f, -90.0f, 0.0f);

	character = new KachujinInstance(instance);

	hp = new HPSystem();
	hp->GetHpbar()->SetParent(instance->GetTransform());
	hp->GetHpbar()->Position(0, 200.0f, 0);

	hp->GetHurtbox()->GetTransform()->SetParent(instance->GetTransform());
	hp->GetHurtbox()->GetTransform()->Position(0.0f, 100.0f, 0.0f);
	hp->GetHurtbox()->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	hp->GetHurtbox()->GetTransform()->Scale(75.0f, 200.0f, 75.0f);

	attack = new EnemyAttackSystem(player);
	attack->GetTransform()->SetParent(instance->GetTransform());
	attack->GetTransform()->Position(0.0f, 100.0f, -40.0f);
	attack->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	attack->GetTransform()->Scale(120.0f, 200.0f, 200.0f);
	attack->SetTag(L"Enemy");

	modelInstancing->UpdateTransforms();
	modelInstancing->UpdateColors();
	modelInstancing->Pass(1);
}

EnemyInstancing::~EnemyInstancing()
{
	SafeDelete(attack);
	SafeDelete(hp);
	SafeDelete(character);
	SafeDelete(modelInstancing);
}

void EnemyInstancing::Update()
{
	Vector3 position;
	character->GetInstance()->GetTransform()->Position(&position);
	if (attack->IsAttackAble(position))
		attack->OnAttack();

	character->Update();
	modelInstancing->Update();
	hp->Update();
	attack->Update();

	modelInstancing->UpdateTransforms();
}

void EnemyInstancing::Render()
{
	modelInstancing->Render();
	hp->Render();
}
