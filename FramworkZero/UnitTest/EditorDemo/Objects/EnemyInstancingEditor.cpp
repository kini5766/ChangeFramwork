#include "stdafx.h"
#include "EnemyInstancingEditor.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Component/TransformEditor.h"
#include "Enemy/EnemyInstancing.h"
#include "Enemy/MeleeEnemy.h"
#include "Enemy/MagicianEnemy.h"

EnemyInstancingEditor::EnemyInstancingEditor(IFocus* player)
	: player(player)
{
	tImGui = new TransformEditor();
}

EnemyInstancingEditor::~EnemyInstancingEditor()
{
	SafeDelete(enemy);
	SafeDelete(tImGui);

	for (auto d : descs)
		SafeDelete(d);
}

void EnemyInstancingEditor::Update()
{
	if (enemy != nullptr)
		enemy->Update();
}

void EnemyInstancingEditor::Render()
{
	if (enemy != nullptr)
		enemy->Render();
}


#pragma region SaveLoad

void EnemyInstancingEditor::Save(BinaryWriter * w)
{
	w->Int(enemyType);

	UINT size = descs.size();
	w->UInt(size);
	for (EnemyInstanceDesc* desc : descs)
	{
		tImGui->Save(&desc->Transform, w);
		w->Vector3(desc->PatrolPoints[0]);
		w->Vector3(desc->PatrolPoints[1]);
	}
}

void EnemyInstancingEditor::Load(BinaryReader * r)
{
	enemyType = r->Int();

	UINT size = descs.size();
	size = r->UInt();
	for (UINT i = 0; i < size; i++)
	{
		EnemyInstanceDesc* desc = new EnemyInstanceDesc();

		tImGui->Load(&desc->Transform, r);
		desc->PatrolPoints.push_back(r->Vector3());
		desc->PatrolPoints.push_back(r->Vector3());

		descs.push_back(desc);
	}
}

bool EnemyInstancingEditor::LoadTakeOut(BinaryReader * r)
{
	CreateEnemy(r->Int());

	UINT size = r->UInt();
	if (size == 0)
		return false;

	for (UINT i = 0; i < size; i++)
	{
		Transform t;
		tImGui->Load(&t, r);
		SetScale(&t);

		Matrix w;
		t.LocalWorld(&w);

		vector<Vector3> patrolPoints;
		patrolPoints.push_back(r->Vector3());
		patrolPoints.push_back(r->Vector3());
		enemy->AddInstance(w, &patrolPoints);
	}

	return true;
}

void EnemyInstancingEditor::CreateEnemy(int item)
{
	enemyType = item;
	switch (item)
	{
	case 0: enemy = new EnemyInstancing(player, unique_ptr<IEnemy>(new MeleeEnemy())); break;
	case 1: enemy = new EnemyInstancing(player, unique_ptr<IEnemy>(new MagicianEnemy(player))); break;
	}

}

void EnemyInstancingEditor::SetScale(Transform * t)
{
	switch (enemyType)
	{
	case 0: t->Scale(0.03f, 0.03f, 0.03f); break;
	case 1: t->Scale(0.025f, 0.025f, 0.025f); break;
	}
}

#pragma endregion


#pragma region Editor

void EnemyInstancingEditor::ImGuiRender()
{
	if (modelTemp == nullptr)
	{
		modelTemp = new Transform();
		modelTemp->Position(0.0f, 90.0f, 0.0f);
		modelTemp->Scale(75.0f, 180.0f, 75.0f);
	}

	// 적 유형 선택
	ImGui::Combo("Enemy Type", &enemyType, enemyTypes);

	// 선택 매쉬 구성요소 수정
	if (selected != -1)
	{
		EnemyInstanceDesc* desc = descs[selected];

		// 선택 매쉬 인스턴스 제거
		if (ImGui::Button("Destroy Instance"))
		{
			int erase = selected;
			Select(-1);
			descs.erase(descs.begin() + erase);
		}
		else
		{
			// 트랜스폼
			Transform* transform = &desc->Transform;
			tImGui->RenderImGui(transform);
			Vector3 position;
			transform->Position(&position);
			if (ImGui::CollapsingHeader("Patrol", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// 라인
				desc->PatrolPoints[0] = position;
				ImGui::DragFloat3("Point", desc->PatrolPoints[1], 0.1f);
				Debug::Line->RenderLine(desc->PatrolPoints[0], desc->PatrolPoints[1], Color(1, 1, 1, 1));
			}
			ImGui::Separator();
		}  // end Destroy else
	}

	// Instance
	if (ImGui::CollapsingHeader("Instance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 매쉬 인스턴스 선택
		for (UINT i = 0; i < descs.size(); i++)
		{
			string imguiName = "Instance " + to_string(i);
			if (selected == i)
				imguiName = ">" + imguiName + "<";

			if (ImGui::Button((imguiName).c_str()))
			{
				Select(i);
			}

			Matrix local;
			modelTemp->LocalWorld(&local);
			modelTemp->SetParent(&descs[i]->Transform);
			modelTemp->LocalWorld(local);
			Debug::Box->RenderBox(modelTemp, Color(1, 0, 1, 1));
		}
	}

	ImGui::Separator();

	// 적 인스턴스 추가
	if (ImGui::Button("+ Add Instance"))
		AddInstance();

}

void EnemyInstancingEditor::On()
{
}

void EnemyInstancingEditor::Off()
{
	Select(-1);
}

void EnemyInstancingEditor::AddInstance()
{
	EnemyInstanceDesc* desc = new EnemyInstanceDesc();
	SetScale(&desc->Transform);
	desc->PatrolPoints.push_back(Vector3(0,0,0));
	desc->PatrolPoints.push_back(Vector3(0,0,1));
	descs.push_back(desc);
}

void EnemyInstancingEditor::Select(int i)
{
	//Matrix local;
	//modelTemp->LocalWorld(&local);
	//modelTemp->SetParent(nullptr);

	selected = i;
	if (i != -1)
		Debug::Gizmo->SetTransform(&descs[selected]->Transform);
}

#pragma endregion
