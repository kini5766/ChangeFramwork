#include "stdafx.h"
#include "WorldPlayer.h"

#include "CharacterController.h"
#include "Component/HPSystem.h"
#include "Component/NormalAttack.h"
#include "Component/AttackAnimation.h"

#include "Character/Kachujin.h"

WorldPlayer::WorldPlayer(Shader * shader)
{
	kachujinMaker = new ModelSkinnedInstancing(shader,
		{
			/*매쉬*/ L"Kachujin/Mesh",
			/*매터리얼*/ L"Kachujin/Mesh",
			/*클립*/ {
				L"Kachujin/Idle",
				L"Kachujin/Walk",
				L"Kachujin/Run",
				L"Kachujin/Slash",
			}
		});

	ModelSkinnedInstance* instance = kachujinMaker->AddInstance();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	instance->GetTransform()->RotationDegree(0.0f, -90.0f, 0.0f);

	kachujin = new KachujinInstance(instance);
	player = new CharacterController
	(
		kachujin->GetInstance()->GetTransform(),
		kachujin->GetAnimator()
	);

	playerHp = new HPSystem();
	playerHp->GetHpbar()->SetParent(instance->GetTransform());
	playerHp->GetHpbar()->Position(0, 200.0f, 0);
	playerHp->GetHurtbox()->SetLayer(
		COLLIDER_LAYER_PLAYER |
		COLLIDER_LAYER_HITBOX
	);
	playerHp->GetHurtbox()->Tag(L"Player");
	playerHp->AddReceiveTag(L"EnemyAttack");

	playerHp->GetHurtbox()->GetTransform()->SetParent(instance->GetTransform());
	playerHp->GetHurtbox()->GetTransform()->Position(0.0f, 100.0f, 0.0f);
	playerHp->GetHurtbox()->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	playerHp->GetHurtbox()->GetTransform()->Scale(75.0f, 200.0f, 75.0f);

	kachujinMaker->UpdateTransforms();
	kachujinMaker->UpdateColors();
	kachujinMaker->Pass(1);

	attack = new NormalAttack();
	attack->InitTransform()->Position(0.0f, 100.0f, -70.0f);
	attack->InitTransform()->Rotation(0.0f, 0.0f, 0.0f);
	attack->InitTransform()->Scale(120.0f, 200.0f, 240.0f);
	attack->Tag(L"PlayerAttack");
	attack->Attack(10.0f);
	attack->ReadyTime(0.25f);
	attack->EndTime(0.5f);
	attack->DelayTime(0.6f);

	player->SetFuncAttack(
		attack->MakeAttackInstance(instance->GetTransform())
	);

	weapon = new Transform();
	weapon->SetParent(instance->GetTransform());

	mesh = new MeshInstancing(shader, new MeshCube());
	mesh->GetRenderer()->GetDefaultMaterial()->Diffuse(Color(0.125f, 0.125f, 0.125f, 1.0f));
	mesh->GetRenderer()->GetDefaultMaterial()->DiffuseMap("Box.png");
	mesh->Pass(0);

	Transform* initWeapon = mesh->AddInstance()->GetTransform();
	initWeapon->SetParent(weapon);
	initWeapon->Position(-2.9f, 1.45f, -6.45f);
	initWeapon->Scale(5.0f, 5.0f, 75.0f);
	initWeapon->Rotation(0, 0, 1);
}

WorldPlayer::~WorldPlayer()
{
	SafeDelete(mesh);
	SafeDelete(weapon);

	SafeDelete(attack);
	SafeDelete(playerHp);
	SafeDelete(player);
	SafeDelete(kachujin);
	SafeDelete(kachujinMaker);
}

void WorldPlayer::Update()
{
	player->Update();

	kachujin->Update();
	kachujinMaker->Update();
	playerHp->Update();
	attack->Update();

	kachujinMaker->UpdateTransforms();
	kachujinMaker->UpdateBoneTracking();

	weapon->LocalWorld(kachujin->GetInstance()->GetAttachBone(40));
	mesh->Update();
	mesh->UpdateTransforms();
}

void WorldPlayer::Render()
{
	mesh->Render();
	kachujinMaker->Render();
	playerHp->Render();
}

IFocus * WorldPlayer::GetFocus()
{
	return player;
}
