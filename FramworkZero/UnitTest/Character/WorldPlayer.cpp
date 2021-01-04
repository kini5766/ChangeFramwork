#include "stdafx.h"
#include "WorldPlayer.h"

#include "Character/Kachujin.h"
#include "Character/CharacterController.h"
#include "Component/HPSystem.h"

WorldPlayer::WorldPlayer(Shader * shader)
{
	kachujin = new ModelSkinnedInstancing(shader,
		{
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

	ModelSkinnedInstance* instance = kachujin->AddInstance();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	instance->GetTransform()->RotationDegree(0.0f, -90.0f, 0.0f);

	kachujin->UpdateTransforms();
	kachujin->UpdateColors();
	kachujin->Pass(1);

	character = new KachujinInstance(instance);
	player = new CharacterController
	(
		character->GetInstance()->GetTransform(),
		character->GetAnimator()
	);

	playerHp = new HPSystem();
	playerHp->GetHpbar()->SetParent(instance->GetTransform());
	playerHp->GetHpbar()->Position(0, 200.0f, 0);

	playerHp->GetHurtbox()->GetTransform()->SetParent(instance->GetTransform());
	playerHp->GetHurtbox()->GetTransform()->Position(0.0f, 100.0f, 0.0f);
	playerHp->GetHurtbox()->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	playerHp->GetHurtbox()->GetTransform()->Scale(75.0f, 200.0f, 75.0f);
}

WorldPlayer::~WorldPlayer()
{
	SafeDelete(playerHp);
	SafeDelete(player);
	SafeDelete(character);
	SafeDelete(kachujin);
}

void WorldPlayer::Update()
{
	character->Update();
	player->Update();
	kachujin->Update();
	playerHp->Update();

	kachujin->UpdateTransforms();
}

void WorldPlayer::Render()
{
	kachujin->Render();
	playerHp->Render();
}

IFocus * WorldPlayer::GetFocus()
{
	return player;
}
