#include "stdafx.h"
#include "TerrainSplattingDemo.h"

#include "Environment/Terrain.h"
#include "Viewer/Freedom.h"

TerrainSplattingDemo::~TerrainSplattingDemo()
{
}

void TerrainSplattingDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 20);

	shader = new Shader(L"14_TerrainSplatting.fx");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Grass (Lawn).jpg");
	terrain->LayerMap(L"Terrain/Snow.jpg", L"Terrain/AlphaMap.png");

	meshShader = new Shader(L"07_Mesh.fx");
}

void TerrainSplattingDemo::Destroy()
{
	SafeDelete(terrain);
	SafeDelete(shader);

	SafeDelete(meshShader);
}

void TerrainSplattingDemo::Update()
{
	// ��
	ImGui::SliderFloat3("d", Context::Get()->Direction(), -1, 1);

	terrain->Update();
}

void TerrainSplattingDemo::Render()
{
	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame) ? 1 : 0);
	terrain->Render();
}

// world ���ϱ� A * B : A�� �ڽ�, B�� �θ�


// ������ ����
// ť�� ���鿡 ���� ����
// orbit(�˵�) ī�޶� (ĳ���� ����ٴϴ� ȸ�� Ȯ�� ��� �ִ밪 ����)