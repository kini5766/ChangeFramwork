#include "stdafx.h"
#include "TerrainDemo.h"

#include "Environment/Terrain.h"

TerrainDemo::~TerrainDemo()
{
}

void TerrainDemo::Initialize()
{
	shader = new Shader(L"06_Terrain.fx");
	terrain = new Terrain(shader, L"Terrain/Red256.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static Vector3 direction = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("d", direction, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(direction);

	terrain->Update();
}

void TerrainDemo::Render()
{
	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame)? 1 : 0);
	terrain->Render();
}

// world 곱하기 A * B : A가 자식, B가 부모