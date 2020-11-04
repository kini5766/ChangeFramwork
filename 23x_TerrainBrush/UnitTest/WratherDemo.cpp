#include "stdafx.h"
#include "WratherDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"
#include "Environment/CubeSky.h"
#include "Environment/Billboard.h"
#include "Environment/Rain.h"
#include "Environment/Snow.h"

WratherDemo::~WratherDemo()
{
}

void WratherDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 20);

	shader = new Shader(L"14_TerrainSplatting.fx");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
	terrain->LayerMap(L"Terrain/Sand (with pebbles).jpg", L"Terrain/Dirt3.png");

	sky = new CubeSky(L"Environment/SunsetCube1024.dds");

	bbShader = new Shader(L"19_Billboard.fx");
	for (UINT i = 0; i < 1000; i++)
	{
		bb[i] = new Billboard(bbShader, L"Terrain/Tree.png");

		Vector2 scale = Math::RandomVec2(3.0f, 10.0f);
		bb[i]->GetTransfrom()->Scale(scale.x, scale.y, 1.0f);

		Vector3 position = Math::RandomVec3(0.0f, 254.0f);
		position.y = terrain->GetHeight(position) + scale.y * 0.5f;
		bb[i]->GetTransfrom()->Position(position);
	}

	rain = new Rain(Vector3(300, 100, 500), 10000, L"Environment/Rain.png");
	snow = new Snow(Vector3(300, 100, 500), 100000, L"Environment/Snow.png");
}

void WratherDemo::Destroy()
{
	SafeDelete(sky);
	SafeDelete(terrain);
	SafeDelete(shader);
	for (UINT i = 0; i < 1000; i++)
		SafeDelete(bb[i]);
	SafeDelete(bbShader);
	SafeDelete(snow);
	SafeDelete(rain);
}

void WratherDemo::Update()
{
	sky->Update();
	terrain->Update();

	for (UINT i = 0; i < 1000; i++)
		bb[i]->Update();

	UINT selected = (UINT)weather;
	ImGui::Separator();
	ImGui::SliderInt("Weather", (int*)&selected, 0, 2);
	weather = (Weather)selected;

	switch (weather)
	{
	case WratherDemo::Weather::Rain: rain->Update(); break;
	case WratherDemo::Weather::Snow: snow->Update(); break;
	}
}

void WratherDemo::Render()
{
	sky->Render();

	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame) ? 1 : 0);
	terrain->Render();

	for (UINT i = 0; i < 1000; i++)
		bb[i]->Render();

	// 항상 반투명은 투명할 수록 마지막에 렌더
	switch (weather)
	{
	case WratherDemo::Weather::Rain: rain->Render(); break;
	case WratherDemo::Weather::Snow: snow->Render(); break;
	}
}

// world 곱하기 A * B : A가 자식, B가 부모
