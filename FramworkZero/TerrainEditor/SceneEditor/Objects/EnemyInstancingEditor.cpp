#include "stdafx.h"
#include "EnemyInstancingEditor.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Component/ComponentTransform.h"

EnemyInstancingEditor::EnemyInstancingEditor()
{
	tImGui = new ComponentTransform();
}

EnemyInstancingEditor::~EnemyInstancingEditor()
{
	SafeDelete(tImGui);

	for (auto d : descs)
		SafeDelete(d);
}

void EnemyInstancingEditor::Update()
{
}

void EnemyInstancingEditor::Render()
{
	// 매쉬 인스턴스 선택
	for (UINT i = 0; i < descs.size(); i++)
	{
		Matrix local;
		modelTemp->LocalWorld(&local);
		modelTemp->SetParent(&descs[i]->Transform);
		modelTemp->LocalWorld(local);
		Debug::Box->RenderBox(modelTemp, Color(1, 0, 1, 1));
	}
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
