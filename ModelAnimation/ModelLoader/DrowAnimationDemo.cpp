#include "stdafx.h"
#include "DrowAnimationDemo.h"
#include "Viewer/Freedom.h"

#include "Model/ModelMesh.h"

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
	{
		static UINT clip;
		static float speed = 1.0f;
		static float takeTime = 1.0f;

		const char* clipName[] = { "Idle", "Walk", "Run", "Slash", "Uprock" };
		ImGui::Text("%s", clipName[clip]);
		ImGui::InputInt("Clip", (int*)&clip);
		clip %= 5;
		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);
		if (ImGui::Button("Apply"))
		{
			kachujin->PlayClip(clip, speed, takeTime);
		}
	}
	kachujin->Update();

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
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/Uprock");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
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

		for (ModelMaterial* material : kachujin->GetModel()->Materials())
			material->SetShader(shader);
		for (ModelMesh* data : kachujin->GetModel()->Meshes())
			data->SetMaterial(kachujin->GetModel()->MaterialByName(data->Data()->PBind->MaterialName));

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
