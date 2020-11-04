#include "stdafx.h"
#include "CpuBillboardDemo.h"
#include "Environment/Terrain.h"
#include "Environment/CubeSky.h"
#include "Viewer/Freedom.h"

#include "Environment/Billboard.h"

CpuBillboardDemo::~CpuBillboardDemo()
{
}

void CpuBillboardDemo::Initialize()
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
}

void CpuBillboardDemo::Destroy()
{
	SafeDelete(sky);
	SafeDelete(terrain);
	SafeDelete(shader);
	for (UINT i = 0; i < 1000; i++)
		SafeDelete(bb[i]);
	SafeDelete(bbShader);
}

void CpuBillboardDemo::Update()
{
	sky->Update();
	terrain->Update();

	for (UINT i = 0; i < 1000; i++)
		bb[i]->Update();
}

void CpuBillboardDemo::Render()
{
	sky->Render();

	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame) ? 1 : 0);
	terrain->Render();

	for (UINT i = 0; i < 1000; i++)
		bb[i]->Render();
}

// world 곱하기 A * B : A가 자식, B가 부모


// 다음주 과제
// 큐브 경사면에 따라 눕기
// orbit(궤도) 카메라 (캐릭터 따라다니는 회전 확대 축소 최대값 존재)