#include "stdafx.h"
#include "MeshDemo.h"

#include "Viewer/Freedom.h"

MeshDemo::~MeshDemo()
{
}

void MeshDemo::Initialize()
{
	//Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	//Context::Get()->GetCamera()->GetTransform()->Position(0, 32, -67);
	//((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"07_Mesh.fx");
	grid = new MeshGrid(shader);
	grid->GetTransform()->Scale(20, 1, 20);
	//grid->Color(0.9f, 0.9f, 0.0f);

	cube = new MeshCube(shader);
	cube->GetTransform()->Scale(20.0f, 10.0f, 20.0f);
	cube->GetTransform()->Position(0, 5, 0);
	//cube->Color(1.0f, 1.0f, 1.0f);

	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->GetTransform()->Position(-30, 6, -15 + (float)i * 15.0f);
		cylinder[i * 2]->GetTransform()->Scale(5, 5, 5);
		//cylinder[i * 2]->Color(0.53f, 0.55f, 0.55f);

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->GetTransform()->Position(30, 6, -15 + (float)i * 15.0f);
		cylinder[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		//cylinder[i * 2 + 1]->Color(0.53f, 0.55f, 0.55f);

		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->GetTransform()->Position(-30, 15.5f, -15 + (float)i * 15.0f);
		sphere[i * 2]->GetTransform()->Scale(5, 5, 5);
		//sphere[i * 2]->Color(0.53f, 0.55f, 0.55f);

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->GetTransform()->Position(+30, 15.5f, -15 + (float)i * 15.0f);
		sphere[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		//sphere[i * 2 + 1]->Color(0.53f, 0.55f, 0.55f);
	}
}

void MeshDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(cube);
	SafeDelete(grid);
	for (UINT i = 0; i < 10; i++)
		SafeDelete(cylinder[i]);
	for (UINT i = 0; i < 10; i++)
		SafeDelete(sphere[i]);
}

void MeshDemo::Update()
{
}

void MeshDemo::Render()
{
	static int pass = 0;
	ImGui::SliderInt("pass", &pass, 0, 1);
	grid->Pass(pass);
	cube->Pass(pass);
	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Pass(pass);
	for (UINT i = 0; i < 10; i++)
		sphere[i]->Pass(pass);

	static Vector3 direction = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("d", direction, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(direction);

	grid->Render();
	cube->Render();

	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Render();
	for (UINT i = 0; i < 10; i++)
		sphere[i]->Render();
}

// world 곱하기 A * B : A가 자식, B가 부모