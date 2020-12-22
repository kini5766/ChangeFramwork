#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	shader = Shader::Load(L"17_Model.fxo");

	Tank();
	Tower();
	Airplane();

	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	planeShader = Shader::Load(L"15_Mesh.fxo");

	plane = new Mesh(planeShader, MeshPlane(6, 6));
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void ModelDemo::Destroy()
{
	SafeRelease(shader);
	SafeDelete(sky);

	SafeRelease(planeShader);
	SafeDelete(plane);

	SafeDelete(tank);
	SafeDelete(airplane);
}

void ModelDemo::Update()
{
	static Vector3 LightDirection = Vector3(-1, -1, +1);
	ImGui::SliderFloat3("LightDirection", LightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
	planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);

	static UINT Pass = 0;
	ImGui::InputInt("Pass", (int*)&Pass);
	Pass %= 2;

	sky->Update();
	plane->Update();

	if (tank != nullptr)
	{
		tank->Pass(Pass);
		tank->Update();
	}

	if (tower != nullptr)
	{
		tower->Pass(Pass);
		tower->Update();
	}

	if (airplane != nullptr)
	{
		airplane->Pass(Pass);
		airplane->Update();
	}
}

void ModelDemo::Render()
{
	sky->Render();
	plane->Render();

	if (tank != nullptr)
		tank->Render();

	if (tower != nullptr)
		tower->Render();

	if (airplane != nullptr)
		airplane->Render();
}

void ModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->SetModel({
		L"Tank/Tank", 
		L"Tank/Tank" }
	);
}

void ModelDemo::Tower()
{
	tower = new ModelRender(shader);
	tower->SetModel({
		L"Tower/Tower",
		L"Tower/Tower" }
	);
	tower->GetTransform()->Position(-5, 0, 0);
	tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}

void ModelDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->SetModel({
		L"B787/Airplane",
		L"B787/Airplane" }
	);
	airplane->GetTransform()->Position(-10, 0, 0);
	airplane->GetTransform()->Rotation(0, 45, 0);
	airplane->GetTransform()->Scale(0.001f, 0.001f, 0.001f);
}

