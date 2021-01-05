#include "stdafx.h"
#include "HPSystem.h"

#include "HPBar.h"

HPSystem::HPSystem()
{
	hurtbox = CollisionManager::Get()->CreateCollider();
	hurtbox->SetLayer(COLLIDER_LAYER_HITBOX);

	hurtReceiver = new ReceiveBox(hurtbox);
	hurtReceiver->AddReceiveTag(L"1 hit");
	hurtReceiver->AddReceiveTag(L"update hit");
	hurtReceiver->AddReceiveTag(L"1 second hit");

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
		if (m.Tag == L"1 hit")
		{
			Debug::Log->Print("30 damage");
			Debug::Log->Print("1 hit!");
			hp -= 30;
		}
		else if (m.Tag == L"1 second hit")
		{
			Debug::Log->Print("10 damage");
			Debug::Log->Print("1 second hit!");
			hp -= 10;
		}
		else if (m.Tag == L"update hit")
		{
			Debug::Log->Show("delta 10 damage");
			Debug::Log->Show("--<update hit!>--");
			c = Color(1.0f, 0.0f, 0.0f, 1.0f);
			hp -= 10 * Time::Delta();
		}
	}
	if (hp < 0.0f)
		hp = 0;

	Debug::Box->RenderBox(hurtbox->GetTransform(), c);
	hpBar->Update(hp, hpMax);

	if (hp == 0.0f)
	{
		Debug::Log->Print("hp zero");
		hpZeroTrigger();
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
