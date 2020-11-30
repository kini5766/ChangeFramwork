#include "stdafx.h"
#include "DrowModelDemo.h"
#include "Viewer/Freedom.h"

#include "Model/ModelMesh.h"

void DrowModelDemo::Initialize()
{
	shader = new Shader(L"18_Animation.fx");
	Context::Get()->GetCamera()->RotationDegree(28, -50, 0);
	Context::Get()->GetCamera()->Position(17, 58, -78);
	//((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	Airplane();
	Tower();
	Tank();
	Kachujin();
}

void DrowModelDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(airplane);
	SafeDelete(tower);
	SafeDelete(tank);
}

void DrowModelDemo::Update()
{
	if (airplane != nullptr) airplane->Update();
	if (tower != nullptr) tower->Update();
	if (kachujin != nullptr) kachujin->Update();

	if (tank != nullptr)
	{
		static UINT boneIndex = 0;
		UINT max = tank->GetModel()->BoneCount();
		ImGui::InputInt("Bone", (int*)&boneIndex);
		boneIndex %= max;

		ModelBone* bone = tank->GetModel()->BoneByIndex(10);
		//ModelBone* bone = tank->GetModel()->BoneByIndex(boneIndex);
		bone->GetTransform()->RotatePitchDegree(Time::Delta() * 15.0f);
		tank->Update();
	}
}

void DrowModelDemo::Render()
{
	ImGui::SliderFloat3("Light", Context::Get()->Direction(), 1, -1);

	if (airplane != nullptr)
	{
		airplane->Pass(1);
		airplane->Render();
	}
	if (tower != nullptr)
	{
		tower->Pass(1);
		tower->Render();
	}
	if (tank != nullptr)
	{
		tank->Pass(1);
		tank->Render();
	}
	if (kachujin != nullptr)
	{
		kachujin->Pass(1);
		kachujin->Render();
	}
}

// ---------- //

void DrowModelDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->ReadMesh(L"B787/Airplane");
	airplane->GetTransform()->Scale(0.005f, 0.005f, 0.005f);
}

void DrowModelDemo::Tower()
{
	tower = new ModelRender(shader);
	tower->ReadMaterial(L"Tower/Tower");
	tower->ReadMesh(L"Tower/Tower");
	tower->GetTransform()->Position(-20, 0, 0);
	tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}

void DrowModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMaterial(L"Tank/Tank");
	tank->ReadMesh(L"Tank/Tank");
	tank->GetTransform()->Position(20, 0, 0);
}

void DrowModelDemo::Kachujin()
{
	kachujin = new ModelRender(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->GetTransform()->Position(3, 0, -20);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}
