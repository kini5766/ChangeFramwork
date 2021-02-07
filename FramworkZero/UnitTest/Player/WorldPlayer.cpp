#include "stdafx.h"
#include "WorldPlayer.h"

#include "CharacterController.h"
#include "Component/HPSystem.h"
#include "Component/NormalAttack.h"
#include "Component/AttackAnimation.h"

#include "Character/Kachujin.h"

WorldPlayer::WorldPlayer()
{
	shaderModel = Shader::Load(URI::Shaders + L"01_Model.fxo");
	shaderMesh = Shader::Load(URI::Shaders + L"01_Mesh.fxo");

	kachujinMaker = new ModelSkinnedInstancing(shaderModel,
		{
			/*매쉬*/ L"Kachujin/Mesh",
			/*매터리얼*/ L"Kachujin/Mesh",
			/*클립*/ {
				L"Kachujin/Idle",
				L"Kachujin/Walk",
				L"Kachujin/Run",
				L"Kachujin/Slash",
				L"Kachujin/Fall",
			}
		}
	);
	kachujinMaker->Pass(0);

	ModelSkinnedInstance* instance = kachujinMaker->AddInstance();
	PlayerAttack();
	Player(instance);
	PlayerHp(instance->GetTransform());
	PlayerWeapon(instance->GetTransform());
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

	SafeRelease(shaderMesh);
	SafeRelease(shaderModel);
}

void WorldPlayer::Update()
{
	player->Update();

	if (bLost)
	{
		if (Input::Keyboard()->Down(VK_SPACE))
		{
			player->Riseup();
			playerHp->RecoveryPer(1);
			bLost = false;
		}
		return;
	}

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
	if (bLost)
		return;

	mesh->Render();
	kachujinMaker->Render();
}

void WorldPlayer::PostRender()
{
	playerHp->PostRender();
}

IFocus * WorldPlayer::GetFocus()
{
	return player;
}

void WorldPlayer::Player(ModelSkinnedInstance* instance)
{
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	instance->GetTransform()->RotationEuler(
		EulerAngle::Degree(0.0f, -90.0f, 0.0f));

	kachujin = new KachujinInstance(instance);
	player = new CharacterController
	(
		kachujin->GetInstance()->GetTransform(),
		kachujin->GetAnimator()
	);

	player->SetFuncAttack(
		attack->MakeAttackInstance(instance->GetTransform())
	);
	player->SetFuncLost([&]() { bLost = true; });

	kachujinMaker->UpdateTransforms();
}

void WorldPlayer::PlayerHp(Transform* transform)
{
	playerHp = new HPSystem();
	playerHp->GetHpbar()->SetParent(transform);
	playerHp->GetHpbar()->Position(0, 200.0f, 0);
	playerHp->GetHurtbox()->SetLayer(
		COLLIDER_LAYER_PLAYER |
		COLLIDER_LAYER_HITBOX
	);
	playerHp->GetHurtbox()->Tag(L"Player");
	playerHp->AddReceiveTag(L"EnemyAttack");

	playerHp->SetFuncDamage(bind(&WorldPlayer::OnDamage, this));

	playerHp->GetHurtbox()->GetTransform()->SetParent(transform);
	playerHp->GetHurtbox()->GetTransform()->Position(0.0f, 100.0f, 0.0f);
	playerHp->GetHurtbox()->GetTransform()->RotationEuler(
		EulerAngle::Degree(0.0f, 0.0f, 0.0f));
	playerHp->GetHurtbox()->GetTransform()->Scale(75.0f, 200.0f, 75.0f);
}

void WorldPlayer::PlayerAttack()
{
	attack = new NormalAttack();
	attack->InitTransform()->Position(0.0f, 100.0f, -70.0f);
	attack->InitTransform()->RotationEuler(
		EulerAngle::Degree(0.0f, 0.0f, 0.0f));
	attack->InitTransform()->Scale(120.0f, 200.0f, 240.0f);
	attack->Tag(L"PlayerAttack");
	attack->Attack(10.0f);
	attack->ReadyTime(0.25f);
	attack->EndTime(0.5f);
	attack->DelayTime(0.6f);
}

void WorldPlayer::PlayerWeapon(Transform* transform)
{
	weapon = new Transform();
	weapon->SetParent(transform);

	mesh = new MeshInstancing(shaderMesh, unique_ptr<MeshData>(new MeshCube()));
	mesh->Pass(1);
	mesh->GetRenderer()->GetDefaultMaterial()->Diffuse(Color(0.125f, 0.125f, 0.125f, 1.0f));
	mesh->GetRenderer()->GetDefaultMaterial()->DiffuseMap("Box.png");

	Transform* initWeapon = mesh->AddInstance()->GetTransform();
	initWeapon->SetParent(weapon);
	initWeapon->Position(-2.9f, 1.45f, -6.45f);
	initWeapon->Scale(5.0f, 5.0f, 75.0f);
	initWeapon->RotationEuler(
		EulerAngle::Degree(0, 0, 1));
}

void WorldPlayer::OnDamage()
{
	if (playerHp->HP() > 0)
		return;

	player->Fall();
}
