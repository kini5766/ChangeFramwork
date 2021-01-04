#include "stdafx.h"
#include "HPSystem.h"

HPSystem::HPSystem()
{
	hpbar = new Render2D();
	hpbar->GetTransform()->Position(0.0f, 25.0f, 0.0f);
	hpbar->GetTransform()->Scale(120.0f, 25.0f, 1.0f);

	hpbarPos = new Transform();

	green = new Texture(L"Green.png");
	hpbar->SRV(green->SRV());

	hurtbox = CollisionManager::Get()->CreateCollider();
	hurtbox->SetLayer(COLLIDER_LAYER_HITBOX);

	hurtReceiver = new ReceiveBox(hurtbox);
	hurtReceiver->AddReceiveTag(L"1 hit");
	hurtReceiver->AddReceiveTag(L"update hit");
	hurtReceiver->AddReceiveTag(L"1 second hit");
}

HPSystem::~HPSystem()
{
	SafeDelete(hurtReceiver);

	SafeDelete(green);
	SafeDelete(hpbarPos);
	SafeDelete(hpbar);
}

void HPSystem::Update()
{
	Matrix W, V, P;
	hpbarPos->LossyWorld(&W);
	V = Context::Get()->View();
	P = Context::Get()->Projection();
	Vector3 pPos = Context::Get()->GetViewport()->
		Project(Vector3(0, 0, 0), W, V, P);

	hpbar->GetTransform()->Position(pPos);
	hpbar->Update();

	hurtReceiver->Update();
	for (BoxReceveDesc& m : hurtReceiver->GetReceived())
	{
		if (m.Tag == L"1 hit")
		{
			Debug::Log->Print("1 hit!");
		}
		else if (m.Tag == L"1 second hit")
		{
			Debug::Log->Print("1 second hit!");
		}
		else if (m.Tag == L"update hit")
		{
			Debug::Log->Show("--<update hit!>--");
		}
	}
	Debug::Box->RenderBox(hurtbox->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

}

void HPSystem::Render()
{
	hpbar->Render();
}
