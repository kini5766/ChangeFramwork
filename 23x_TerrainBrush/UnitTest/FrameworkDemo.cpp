#include "stdafx.h"
#include "FrameworkDemo.h"
#include "Environment/Terrain.h"

#include "Viewer/Freedom.h"

FrameworkDemo::~FrameworkDemo()
{
}

void FrameworkDemo::Initialize()
{
	//Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	//Context::Get()->GetCamera()->GetTransform()->Position(0, 32, -67);
	//((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"10_Mesh.fx");
	grid = new MeshGrid(shader, 5.0f, 5.0f);
	grid->GetTransform()->Scale(20, 1, 20);
	grid->DiffuseMap(L"Floor.png");

	cube = new MeshCube(shader);
	cube->GetTransform()->Scale(20.0f, 10.0f, 20.0f);
	cube->GetTransform()->Position(0, 5, 0);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->GetTransform()->Position(-30, 6, -15 + (float)i * 15.0f);
		cylinder[i * 2]->GetTransform()->Scale(5, 5, 5);
		cylinder[i * 2]->DiffuseMap(L"Bricks.png");

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->GetTransform()->Position(30, 6, -15 + (float)i * 15.0f);
		cylinder[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinder[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->GetTransform()->Position(-30, 15.5f, -15 + (float)i * 15.0f);
		sphere[i * 2]->GetTransform()->Scale(5, 5, 5);
		sphere[i * 2]->DiffuseMap(L"Wall.png");

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->GetTransform()->Position(+30, 15.5f, -15 + (float)i * 15.0f);
		sphere[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		sphere[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}

	terrainShader = new Shader(L"09_Terrain.fx");
	terrain = new Terrain(terrainShader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void FrameworkDemo::Destroy()
{
	SafeDelete(terrainShader);
	SafeDelete(terrain);
	SafeDelete(shader);
	SafeDelete(cube);
	SafeDelete(grid);
	for (UINT i = 0; i < 10; i++)
		SafeDelete(cylinder[i]);
	for (UINT i = 0; i < 10; i++)
		SafeDelete(sphere[i]);
}

void FrameworkDemo::Update()
{
	terrain->Update();
}

void FrameworkDemo::Render()
{
	static int pass = 0;
	ImGui::SliderInt("pass", &pass, 0, 1);
	grid->Pass(pass);
	cube->Pass(pass);
	terrain->Pass(pass);
	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Pass(pass);
	for (UINT i = 0; i < 10; i++)
		sphere[i]->Pass(pass);

	ImGui::SliderFloat3("d", Context::Get()->Direction(), -1, 1);

	grid->Render();
	cube->Render();

	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Render();
	for (UINT i = 0; i < 10; i++)
		sphere[i]->Render();

	terrain->Render();
}

// world 곱하기 A * B : A가 자식, B가 부모