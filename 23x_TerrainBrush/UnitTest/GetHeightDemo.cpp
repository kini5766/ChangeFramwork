#include "stdafx.h"
#include "GetHeightDemo.h"

#include "Environment/Terrain.h"
#include "Viewer/Freedom.h"

GetHeightDemo::~GetHeightDemo()
{
}

void GetHeightDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 20);

	shader = new Shader(L"09_Terrain.fx");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");

	meshShader = new Shader(L"07_Mesh.fx");
	sphere = new MeshSphere(meshShader, 0.5f);
	//sphere->Color(1, 0, 0);
}

void GetHeightDemo::Destroy()
{
	SafeDelete(terrain);
	SafeDelete(shader);

	SafeDelete(sphere);
	SafeDelete(meshShader);
}

void GetHeightDemo::Update()
{
	// 빛
	static Vector3 direction = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("d", direction, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(direction);
	meshShader->AsVector("LightDirection")->SetFloatVector(direction);

	// 이동
	Vector3 pos;
	sphere->GetTransform()->Position(&pos);

	if (Keyboard::Get()->Press(VK_UP))
		pos.z += 20.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		pos.z -= 20.0f * Time::Delta();

	if (Keyboard::Get()->Press(VK_RIGHT))
		pos.x += 20.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		pos.x -= 20.0f * Time::Delta();

	//pos.y = terrain->GetHeight(pos);
	pos.y = terrain->GetHeightPick(pos);

	Vector3 scale;
	sphere->GetTransform()->Scale(&scale);
	pos.y += scale.y * 0.5f;

	sphere->GetTransform()->Position(pos);

	terrain->Update();
}

void GetHeightDemo::Render()
{
	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame) ? 1 : 0);
	terrain->Render();

	sphere->Render();
}

// world 곱하기 A * B : A가 자식, B가 부모


// 다음주 과제
// 큐브 경사면에 따라 눕기
// orbit(궤도) 카메라 (캐릭터 따라다니는 회전 확대 축소 최대값 존재)