#include "stdafx.h"
#include "EnemyInstancing.h"

#include "Tools/Viewer/IFocus.h"
#include "Objects/Model/ModelAnimation.h"

#include "Character/Paladin.h"
#include "Component/NormalAttack.h"
#include "Component/AttackAnimation.h"
#include "EnemyInstance.h"


EnemyInstancing::EnemyInstancing(Shader * shader, IFocus* player)
	: player(player)
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

	normalAttack = new NormalAttack();
	normalAttack->InitTransform()->Position(0.0f, 90.0f, -40.0f);
	normalAttack->InitTransform()->Rotation(0.0f, 0.0f, 0.0f);
	normalAttack->InitTransform()->Scale(100.0f, 180.0f, 180.0f);
	normalAttack->Tag(L"Enemy");


	AddInstance();

	modelInstancing->UpdateTransforms();
	modelInstancing->UpdateColors();
	modelInstancing->Pass(1);
}

EnemyInstancing::~EnemyInstancing()
{
	SafeDelete(normalAttack);
	SafeDelete(instance);
	SafeDelete(modelInstancing);
}

void EnemyInstancing::Update()
{
	instance->Update();
	modelInstancing->Update();
	modelInstancing->UpdateTransforms();
	normalAttack->Update();
}

void EnemyInstancing::Render()
{
	modelInstancing->Render();
	instance->Render();
}

void EnemyInstancing::AddInstance()
{
	EnemyDesc desc;
	ModelSkinnedInstance* i = modelInstancing->AddInstance();

	Transform* t = i->GetTransform();
	t->Scale(0.03f, 0.03f, 0.03f);
	t->Position(25.0f, 0.0f, 25.0f);
	t->RotationDegree(0.0f, -90.0f, 0.0f);
	desc.Transform = t;

	Animator* anim = new Animator();
	Paladin::BindAnimation(anim, i->GetAnimation());
	desc.Animator = anim;

	desc.Attack = normalAttack->MakeInstance(t);

	instance = new EnemyInstance(desc, player);
}
