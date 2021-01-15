#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	shader = Shader::Load(L"01_Material.fxo");

	Tank();
	Tower();
	Airplane();
}

void ModelDemo::Destroy()
{
	SafeRelease(shader);

	SafeDelete(tank);
	SafeDelete(airplane);
}

void ModelDemo::Update()
{
	if (tank != nullptr)
		tank->Update();

	if (tower != nullptr)
		tower->Update();

	if (airplane != nullptr)
		airplane->Update();
}

void ModelDemo::Render()
{
	if (tank != nullptr)
		tank->Render();

	if (tower != nullptr)
		tower->Render();

	if (airplane != nullptr)
		airplane->Render();
}

void ModelDemo::Tank()
{
	tank = new ModelSkinnedInstancing(shader, {
		L"Tank/Tank", 
		L"Tank/Tank" }
	);

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tank->AddInstance()->GetTransform();
		transform->Position(Vector3(x, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-179.9f, 179.9f), 0);
		transform->Scale(0.3f, 0.3f, 0.3f);
	}

	tank->UpdateTransforms();
	tank->Pass(1);
}

void ModelDemo::Tower()
{
	tower = new ModelSkinnedInstancing(shader, {
		L"Tower/Tower",
		L"Tower/Tower" }
	);

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tower->AddInstance()->GetTransform();
		transform->Position(Vector3(x, 0.0f, 7.0f));
		transform->RotationDegree(0, Math::Random(-179.9f, 179.9f), 0);
		transform->Scale(0.003f, 0.003f, 0.003f);
	}

	tower->UpdateTransforms();
	tower->Pass(1);
}

void ModelDemo::Airplane()
{
	airplane = new ModelSkinnedInstancing(shader, {
		L"B787/Airplane",
		L"B787/Airplane" }
	);

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = airplane->AddInstance()->GetTransform();
		transform->Position(Vector3(x, 0.0f, 2.5f));
		transform->RotationDegree(0, Math::Random(-179.9f, 179.9f), 0);
		transform->Scale(0.0003f, 0.0003f, 0.0003f);
	}

	airplane->UpdateTransforms();
	airplane->Pass(1);
}

