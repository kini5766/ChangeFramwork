#include "stdafx.h"
#include "RaycastDemo.h"

#include "stdafx.h"
#include "GetHeightDemo.h"

#include "Environment/Terrain.h"
#include "Viewer/Freedom.h"

RaycastDemo::~RaycastDemo()
{
}

void RaycastDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	//Context::Get()->GetCamera()->GetTransform()->Position(132, 42, -17);
	Context::Get()->GetCamera()->Position(-2, 32, 94);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"09_Terrain.fx");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");

	meshShader = new Shader(L"07_Mesh.fx");
	sphere = new MeshSphere(meshShader, 0.5f);
	//sphere->Color(1, 0, 0);
}

void RaycastDemo::Destroy()
{
	SafeDelete(terrain);
	SafeDelete(shader);

	SafeDelete(sphere);
	SafeDelete(meshShader);
}

void RaycastDemo::Update()
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

void RaycastDemo::Render()
{
	static bool bWireFame = false;
	ImGui::Checkbox("WireFame", &bWireFame);
	terrain->Pass((bWireFame) ? 1 : 0);

	Vector3 picked = terrain->GetPickedPosition();
	sphere->GetTransform()->Position(picked);
	ImGui::LabelText("picked", "%0.2f, %0.2f, %0.2f", picked.x, picked.y, picked.z);
	//string str = to_string(picked.x);
	//str = ", " + to_string(picked.y);
	//str = ", " + to_string(picked.z);
	//Gui::Get()->RenderText(, , 1, 1, 1, str);


	Matrix w, v, p;
	D3DXMatrixIdentity(&w);
	v = Context::Get()->View();
	p = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();
	Vector3 result, position;
	position = picked;
	//sphere->GetTransform()->Position(&position);
	//position.y = terrain->GetHeight(position) + 0.5f;
	vp->Project(&result, position, w, v, p);
	Gui::Get()->RenderText(result.x, result.y, 1, 1, 1, "RaycastDemo");

	terrain->Render();
	sphere->Render();

	ImGui::LabelText("VP", "%0.2f, %0.2f, %0.2f", result.x, result.y, result.z);
}

// world 곱하기 A * B : A가 자식, B가 부모


// 다음주 과제
// 큐브 경사면에 따라 눕기
// orbit(궤도) 카메라 (캐릭터 따라다니는 회전 확대 축소 최대값 존재)