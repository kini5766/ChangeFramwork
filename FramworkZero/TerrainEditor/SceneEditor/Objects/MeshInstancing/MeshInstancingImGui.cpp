#include "stdafx.h"
#include "MeshInstancingImGui.h"

#include "MeshInstancingDesc.h"

MeshInstancingImGui::MeshInstancingImGui(MeshInstancingDesc * desc)
	: desc(desc)
{
}

MeshInstancingImGui::~MeshInstancingImGui()
{
}

void MeshInstancingImGui::ImGuiRender()
{

	if (desc->Mesh == nullptr)
	{
		// �Ž� ���� ����
		MakingInstancing();
		ImGui::Separator();
		desc->MImGui->RenderImGui();
		return;
	}

	// ���� �Ž� ������� ����
	if (selected != -1)
	{
		vector<ColliderBox*>& colliders = desc->Colliders;
		vector<MeshInstance*>& meshes = desc->Meshes;

		MeshInstance* mesh = meshes[selected];
		ImGui::Text(("Instance " + to_string(selected)).c_str());

		// ���� �Ž� �ν��Ͻ� ����
		if (ImGui::Button("Destroy Instance"))
		{
			int erase = selected;
			Select(-1);
			mesh->Release();
			if (colliders[erase] != nullptr)
				colliders[erase]->Release();
			colliders.erase(colliders.begin() + erase);
			meshes.erase(meshes.begin() + erase);
		}
		else
		{
			// Ʈ������
			Transform* transform = mesh->GetTransform();
			desc->TImGui->RenderImGui(transform);

			// �ݶ��̴�
			ColliderBox* collider = colliders[selected];
			if (collider != nullptr)
			{
				if (ImGui::Button("Delete Collider"))
				{
					SafeRelease(collider);
					colliders[selected] = nullptr;
				}
				else
				{
					desc->CImGui->RenderImGui(collider);
				}
			}
			else if (ImGui::Button("Create Collider"))
			{
				desc->AddCollider(selected);
			}

			ImGui::Separator();

		}  // end Destroy else

	}


	if (ImGui::CollapsingHeader("Instance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �Ž� �ν��Ͻ� ����
		for (UINT i = 0; i < desc->Meshes.size(); i++)
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

	// �Ž� �ν��Ͻ� �߰�
	if (ImGui::Button("+ Add Instance"))
	{
		AddInstance();
	}

	ImGui::Separator();
	desc->MImGui->RenderImGui();
	if (ImGui::Button("Apply Material"))
	{
		desc->MImGui->Apply(desc->Mesh->GetMaterial());
	}

	desc->Mesh->UpdateTransforms();
}

void MeshInstancingImGui::AddInstance()
{
	int index = desc->Meshes.size();
	desc->AddInstance();
	Select(index);
}

void MeshInstancingImGui::Select(int i)
{
	if (selected == i || i == -1)
	{
		selected = -1;
		Debug::Gizmo->SetTransform(nullptr);
	}
	else
	{
		selected = i;
		Transform* tansform = desc->Meshes[selected]->GetTransform();
		Debug::Gizmo->SetTransform(tansform);
	}
}

void MeshInstancingImGui::MakingInstancing()
{
	float& _f1 = desc->_F1, _f2 = desc->_F2;
	UINT& _u1 = desc->_U1, _u2 = desc->_U2;

	if (ImGui::Combo("Mesh", &imguiItem, meshItems))
	{
		switch (imguiItem)
		{
		case 0: /* Cube */ break;
		case 1: /* Cylinder */ _f1 = 0.5f; _f2 = 3.0f; _u1 = 20; _u2 = 20; break;
		case 2: /* Plane */ _f1 = 2.5f; _f2 = 2.5f; break;
		case 3: /* Quad */ break;
		case 4: /* Sphere */ _f1 = 0.5f; _u1 = 20; _u2 = 20; break;
		}
	}

	switch (imguiItem)
	{
	case 0:
	{
		if (ImGui::Button("Create Cube"))
		{
			desc->MeshType = imguiItem;
			desc->SetInstancing();
			AddInstance();
		}
	}break;

	case 1:
	{
		ImGui::InputFloat("Radius", &_f1);
		ImGui::InputFloat("Height", &_f2);
		ImGui::InputInt("StackCount", (int*)&_u1);
		ImGui::InputInt("SliceCount", (int*)&_u2);
		if (ImGui::Button("Create Cylinder"))
		{
			desc->MeshType = imguiItem;
			desc->SetInstancing();
			AddInstance();
		}
	}break;

	case 2:
	{
		ImGui::InputFloat("OffsetU", &_f1);
		ImGui::InputFloat("OffsetV", &_f2);
		if (ImGui::Button("Create Plane"))
		{
			desc->MeshType = imguiItem;
			desc->SetInstancing();
			AddInstance();
		}
	}break;

	case 3:
	{
		if (ImGui::Button("Create Quad"))
		{
			desc->MeshType = imguiItem;
			desc->SetInstancing();
			AddInstance();
		}
	}break;

	case 4:
	{
		ImGui::InputFloat("Radius", &_f1);
		ImGui::InputInt("StackCount", (int*)&_u1);
		ImGui::InputInt("SliceCount", (int*)&_u2);
		if (ImGui::Button("Create Sphere"))
		{
			desc->MeshType = imguiItem;
			desc->SetInstancing();
			AddInstance();
		}
	}break;

	}
}
