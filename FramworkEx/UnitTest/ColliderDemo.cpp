#include "stdafx.h"
#include "ColliderDemo.h"

ColliderDemo::~ColliderDemo()
{
}

void ColliderDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 3, -8);

	shader = new Shader(L"17_Model.fx");

	gridMaterial = new Material(shader);
	gridMaterial->DiffuseMap(L"Black.png");

	grid = new MeshGrid(shader);

	t1 = new Transform();
	t1->Position(0.0f, 0.5f, 0.0f);
	t1->Scale(2, 1, 1);
	collider1 = new Collider(t1);

	t2 = new Transform();
	t2->Position(1.0f, 0.5f, 0.0f);
	collider2 = new Collider(t2);
}

void ColliderDemo::Destroy()
{
	SafeDelete(collider2);
	SafeDelete(t2);
	SafeDelete(collider1);
	SafeDelete(t1);
	SafeDelete(grid);
	SafeDelete(gridMaterial);
	SafeDelete(shader);
}

void ColliderDemo::Update()
{
	grid->Update();

	Vector3 pos2;
	t2->Position(&pos2);
	if (Keyboard::Get()->Press(VK_LSHIFT) == false)
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			pos2.x += 3.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			pos2.x -= 3.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			pos2.z += 3.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			pos2.z -= 3.0f * Time::Delta();
	}
	t2->Position(pos2);

	Vector3 rotation1;
	Vector3 rotation2;

	t1->RotationDegree(&rotation1);
	t2->RotationDegree(&rotation2);

	ImGui::DragFloat3("Rotation1", rotation1, 1.0f, -179.9f, 179.9f);
	ImGui::DragFloat3("Rotation2", rotation2, 1.0f, -179.9f, 179.9f);

	t1->RotationDegree(rotation1);
	t2->RotationDegree(rotation2);

	collider1->Update();
	collider2->Update();
}

void ColliderDemo::Render()
{
	gridMaterial->Render();
	grid->Render();

	bool bCheck = collider1->IsIntersect(collider2);
	Color c = Color(0xff00ff00);
	if (bCheck)
	{
		c = Color(0xffff0000);
	}

	collider1->Render(c);
	collider2->Render(c);
}
