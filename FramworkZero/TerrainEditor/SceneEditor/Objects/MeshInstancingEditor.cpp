#include "stdafx.h"
#include "MeshInstancingEditor.h"

#include "Component/ComponentTransform.h"
#include "Component/ComponentCollider.h"
#include "Component/ComponentMaterial.h"

MeshInstancingEditor::MeshInstancingEditor()
{
	shader = Shader::Load(L"01_Mesh.fxo");
	tImGui = new ComponentTransform();
	cImGui = new ComponentCollider();
	mImGui = new ComponentMaterial();
}

MeshInstancingEditor::~MeshInstancingEditor()
{
	SafeDelete(mImGui);
	SafeDelete(cImGui);
	SafeDelete(tImGui);
	SafeDelete(meshInstancing);
	SafeRelease(shader);
}

void MeshInstancingEditor::Update()
{
	if (meshInstancing == nullptr)
		return;

	meshInstancing->Update();
}

void MeshInstancingEditor::Render()
{
	if (meshInstancing == nullptr)
		return;

	meshInstancing->Render();

	for (ColliderBox* collider: colliders)
		if (collider != nullptr)
			Debug::Box->RenderBox(collider->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
}


#pragma region Save Load

void MeshInstancingEditor::Save(BinaryWriter * w)
{
	if (meshInstancing == nullptr)
	{
		w->Int(-1);
		return;
	}
	w->Int(meshType);

	// meshInstancing
	w->Float(_f1);
	w->Float(_f2);
	w->UInt(_u1);
	w->UInt(_u2);
	mImGui->Save(w);

	// meshInstance
	UINT size = meshes.size();
	w->UInt(size);
	for (UINT i = 0; i < size; i++)
	{
		MeshInstance* mesh = meshes[i];
		tImGui->Save(mesh->GetTransform(), w);
		ColliderBox* collider = colliders[i];

		bool hasCollider = collider != nullptr;
		w->Bool(hasCollider);
		if (hasCollider)
			cImGui->Save(collider, w);
	}
}

void MeshInstancingEditor::Load(BinaryReader * r)
{
	imguiItem = r->Int();
	if (imguiItem == -1) return;

	// meshInstancing
	_f1 = r->Float();
	_f2 = r->Float();
	_u1 = r->UInt();
	_u2 = r->UInt();

	mImGui->Load(r);

	LoadInstancing(imguiItem);

	// meshInstance
	UINT size = r->UInt();
	for (UINT i = 0; i < size; i++)
	{
		AddInstance();

		MeshInstance* mesh = meshes[i];
		tImGui->Load(mesh->GetTransform(), r);

		bool hasCollider = r->Bool();
		if (hasCollider)
			cImGui->Load(AddCollider(i), r);
	}

	meshInstancing->UpdateTransforms();
}

#pragma endregion


#pragma region Editor

void MeshInstancingEditor::ImGuiRender()
{
	if (meshInstancing == nullptr)
	{
		// 매쉬 유형 선택
		MakingInstancing();
		ImGui::Separator();
		mImGui->RenderImGui();
		return;
	}

	// 선택 매쉬 구성요소 수정
	if (selected != -1)
	{
		MeshInstance* mesh = meshes[selected];
		ImGui::Text(("Instance " + to_string(selected)).c_str());

		// 선택 매쉬 인스턴스 제거
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
			// 트랜스폼
			Transform* transform = mesh->GetTransform();
			tImGui->RenderImGui(transform);

			// 콜라이더
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
					cImGui->RenderImGui(collider);
				}
			}
			else if (ImGui::Button("Create Collider"))
			{
				AddCollider(selected);
			}

			ImGui::Separator();

		}  // end Destroy else

	}


	if (ImGui::CollapsingHeader("Instance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 매쉬 인스턴스 선택
		for (UINT i = 0; i < meshes.size(); i++)
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

	// 매쉬 인스턴스 추가
	if (ImGui::Button("+ Add Instance"))
	{
		AddInstance();
	}

	ImGui::Separator();
	mImGui->RenderImGui();
	if (ImGui::Button("Apply Material"))
	{
		mImGui->Apply(meshInstancing->GetRenderer()->GetDefaultMaterial());
	}

	meshInstancing->UpdateTransforms();
}

void MeshInstancingEditor::On()
{
	Select(0);
}

void MeshInstancingEditor::Off()
{
	Select(-1);
}

void MeshInstancingEditor::Select(int i)
{
	if (selected == i || i == -1)
	{
		selected = -1;
		Debug::Gizmo->SetTransform(nullptr);
	}
	else
	{
		selected = i;
		Transform* tansform = meshes[selected]->GetTransform();
		Debug::Gizmo->SetTransform(tansform);
	}
}

#pragma endregion


#pragma region Create

void MeshInstancingEditor::AddInstance()
{
	int index = meshes.size();
	meshes.push_back(meshInstancing->AddInstance());
	colliders.push_back(nullptr);

	Select(index);
}

ColliderBox* MeshInstancingEditor::AddCollider(UINT index)
{
	ColliderBox* collider = CollisionManager::Get()->CreateCollider();

	Transform* transform = meshes[index]->GetTransform();
	collider->GetTransform()->SetParent(transform);
	Matrix w;
	D3DXMatrixIdentity(&w);
	collider->GetTransform()->LocalWorld(w);

	colliders[index] = collider;
	return collider;
}

void MeshInstancingEditor::MakingInstancing()
{
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
			LoadInstancing(imguiItem);
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
			LoadInstancing(imguiItem);
			AddInstance();
		}
	}break;

	case 2:
	{
		ImGui::InputFloat("OffsetU", &_f1);
		ImGui::InputFloat("OffsetV", &_f2);
		if (ImGui::Button("Create Plane"))
		{
			LoadInstancing(imguiItem);
			AddInstance();
		}
	}break;

	case 3:
	{
		if (ImGui::Button("Create Quad"))
		{
			LoadInstancing(imguiItem);
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
			LoadInstancing(imguiItem);
			AddInstance();
		}
	}break;

	}

}

void MeshInstancingEditor::LoadInstancing(UINT item)
{
	switch (item)
	{
	case 0: SetInstancing(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshCube()))); break;
	case 1: SetInstancing(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshCylinder(_f1, _f2, _u1, _u2)))); break;
	case 2: SetInstancing(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshPlane(_f1, _f2)))); break;
	case 3: SetInstancing(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshQuad()))); break;
	case 4: SetInstancing(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshSphere(_f1, _u1, _u2)))); break;
	}
}

void MeshInstancingEditor::SetInstancing(MeshInstancing * value)
{
	meshInstancing = value;
	meshInstancing->Pass(1);
	mImGui->Apply(value->GetRenderer()->GetDefaultMaterial());
	meshType = imguiItem;
}

#pragma endregion

