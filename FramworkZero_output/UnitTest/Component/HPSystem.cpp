#include "stdafx.h"
#include "HPSystem.h"

#include "HPBar.h"
#include "Component/AttackSystem.h"

HPSystem::HPSystem(float hpMax)
	: hpMax(hpMax), hp(hpMax)
{
	hurtbox = CollisionManager::Get()->CreateCollider();
	hurtbox->SetLayer(COLLIDER_LAYER_HITBOX);

	hurtReceiver = new ReceiveBox(hurtbox);

	hpBar = new HPBar();
}

HPSystem::~HPSystem()
{
	SafeDelete(hpBar);
	SafeDelete(hurtReceiver);
}

void HPSystem::Update()
{
	if (hp == 0.0f)
		return;

	hurtReceiver->Update();
	Color c(0.0f, 1.0f, 0.0f, 1.0f);
	for (BoxReceveDesc& m : hurtReceiver->GetReceived())
	{
		AttackDesc* attack = (AttackDesc*)m.Data;
		hp -= attack->Attack;

		Debug::Log->Print("Attack : " + to_string((int)attack->Attack));
		Debug::Log->Print("Tag : " + String::ToString(m.Tag));

		funcDamage();
	}

	if (hp < 0.0f)
		hp = 0;

	Debug::Box->RenderBox(hurtbox->GetTransform(), c);
	hpBar->Update(hp, hpMax);

	if (hp == 0.0f)
	{
		Debug::Log->Print("hp zero");
	}
}

void HPSystem::Render()
{
	if (hp == 0.0f)
		return;

	hpBar->Render();
}

Transform * HPSystem::GetHpbar()
{
	return hpBar->GetTransform();
}

void HPSystem::AddReceiveTag(wstring value)
{
	hurtReceiver->AddReceiveTag(value);
}

void HPSystem::RecoveryPer(float per)
{
	hp += hpMax * per;
	if (hp > hpMax)
		hp = hpMax;
}
