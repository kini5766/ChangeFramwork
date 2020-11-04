#include "stdafx.h"
#include "DrowAnimationDemo.h"
#include "Viewer/Freedom.h"

void DrowAnimationDemo::Initialize()
{
	shader = new Shader(L"18_Animation.fx");

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -50);
	//((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	Kachujin();
	Paladin();
}

void DrowAnimationDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(paladin);
	SafeDelete(kachujin);
	//SafeDelete(weapon);
}

void DrowAnimationDemo::Update()
{
	static UINT clip = 0;
	if (kachujin != nullptr)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			++clip;
			clip %= 5;
			kachujin->PlayClip(clip);
		}

		kachujin->Update();
	}

	if (paladin != nullptr) paladin->Update();
}

void DrowAnimationDemo::Render()
{
	ImGui::SliderFloat3("Light", Context::Get()->Direction(), 1, -1);

	if (kachujin != nullptr)
	{
		kachujin->Pass(2);
		kachujin->Render();
	}
	if (paladin != nullptr)
	{
		paladin->Pass(2);
		paladin->Render();
	}
}

// ---------- //

void DrowAnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Paladin/SwingDancing");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	kachujin->GetTransform()->Position(3, 0, -20);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	
	// Attach weapon
	{
		Model* weapon = nullptr;
		weapon = new Model();

		weapon->ReadMaterial(L"Weapon/Sword");
		weapon->ReadMesh(L"Weapon/Sword");

		Transform offset = Transform();
		offset.Position(-13, -3, -5);
		offset.RotationDegree(0, 0, 90);
		offset.Scale(0.5f, 0.5f, 5.5f);
		kachujin->GetModel()->Attach(shader, weapon, 35, &offset);

		SafeDelete(weapon);
	}
}

void DrowAnimationDemo::Paladin()
{
	paladin = new ModelAnimator(shader);
	paladin->ReadMaterial(L"Paladin/Mesh");
	paladin->ReadMesh(L"Paladin/Mesh");

	paladin->ReadClip(L"Paladin/Taunt");
	paladin->ReadClip(L"Paladin/Dancing");
	paladin->ReadClip(L"Paladin/SwingDancing");

	paladin->GetTransform()->Position(-3, 0, -20);
	paladin->GetTransform()->Scale(0.03f, 0.03f, 0.03f);
}
