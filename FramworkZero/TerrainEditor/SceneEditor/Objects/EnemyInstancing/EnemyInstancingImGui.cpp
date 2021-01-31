#include "stdafx.h"
#include "EnemyInstancingImGui.h"

#include "EnemyInstancingDesc.h"

EnemyInstancingImGui::EnemyInstancingImGui(EnemyInstancingDesc * desc)
	: desc(desc)
{
}

EnemyInstancingImGui::~EnemyInstancingImGui()
{
}

void EnemyInstancingImGui::ImGuiRender()
{
	// �� ���� ����
	ImGui::Combo("Enemy Type", &desc->EnemyType, enemyTypes);

	// ���� �Ž� ������� ����
	if (selected != -1)
	{
		EnemyInstanceDesc* unit = desc->Units[selected];

		// ���� �Ž� �ν��Ͻ� ����
		if (ImGui::Button("Destroy Instance"))
		{
			int erase = selected;
			Select(-1);
			desc->Units.erase(desc->Units.begin() + erase);
		}
		else
		{
			// Ʈ������
			Transform* transform = &unit->Transform;
			desc->TImGui->RenderImGui(transform);
			Vector3 position;
			transform->Position(&position);
			if (ImGui::CollapsingHeader("Patrol", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// ����
				unit->PatrolPoints[0] = position;
				ImGui::DragFloat3("Point", unit->PatrolPoints[1], 0.1f);
				Debug::Line->RenderLine(unit->PatrolPoints[0], unit->PatrolPoints[1], Color(1, 1, 1, 1));
			}
			ImGui::Separator();
		}  // end Destroy else
	}

	// Instance
	if (ImGui::CollapsingHeader("Instance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �Ž� �ν��Ͻ� ����
		for (UINT i = 0; i < desc->Units.size(); i++)
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

	// �� �ν��Ͻ� �߰�
	if (ImGui::Button("+ Add Instance"))
		desc->AddInstance();
}

void EnemyInstancingImGui::Select(int i)
{
	selected = i;
	if (i != -1)
		Debug::Gizmo->SetTransform(&desc->Units[selected]->Transform);
}
