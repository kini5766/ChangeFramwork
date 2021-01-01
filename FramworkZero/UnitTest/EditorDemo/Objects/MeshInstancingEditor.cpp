#include "stdafx.h"
#include "MeshInstancingEditor.h"

#include "TransformEditor.h"
#include "ColliderEditor.h"
#include "Utilities/BinaryFile.h"

MeshInstancingEditor::MeshInstancingEditor()
{
	shader = Shader::Load(L"01_Material.fxo");
	tImGui = new TransformEditor();
	cImGui = new ColliderEditor();
}

MeshInstancingEditor::~MeshInstancingEditor()
{
	SafeDelete(cImGui);
	SafeDelete(tImGui);
	SafeDelete(meshInstancing);
	SafeRelease(shader);
}

void MeshInstancingEditor::ImGuiRender()
{
	// 매쉬 유형 선택
	if (meshInstancing == nullptr)
	{
		MakingInstancing();
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

		// 트랜스폼
		Transform* transform = mesh->GetTransform();
		tImGui->RenderImGui(transform);

		// 콜라이더
		Collider* collider = colliders[selected];
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
	}


	if (ImGui::CollapsingHeader("Instance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 매쉬 인스턴스 선택
		for (UINT i = 0; i < meshes.size(); i++)
		{
			if (ImGui::Button(("Instance " + to_string(i)).c_str()))
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
}

void MeshInstancingEditor::Update()
{
	if (meshInstancing == nullptr)
		return;

	meshInstancing->UpdateTransforms();
	meshInstancing->Update();
}

void MeshInstancingEditor::Render()
{
	if (meshInstancing == nullptr)
		return;

	meshInstancing->Render();
}

void MeshInstancingEditor::Save(BinaryWriter * w)
{
	if (meshInstancing == nullptr)
	{
		w->Int(-1);
		return;
	}
	w->Int(item);

	// meshInstancing
	w->Float(_f1);
	w->Float(_f2);
	w->UInt(_u1);
	w->UInt(_u2);
	w->String(String::ToString(diffuse));

	// meshInstance
	UINT size = meshes.size();
	w->UInt(size);
	for (UINT i = 0; i < size; i++)
	{
		MeshInstance* mesh = meshes[i];
		tImGui->Save(mesh->GetTransform(), w);
		Collider* collider = colliders[i];

		bool hasCollider = collider != nullptr;
		w->Bool(hasCollider);
		if (hasCollider)
			cImGui->Save(collider, w);
	}
}

void MeshInstancingEditor::Load(BinaryReader * r)
{
	int tem = r->Int();
	if (tem == -1) return;

	// meshInstancing
	_f1 = r->Float();
	_f2 = r->Float();
	_u1 = r->UInt();
	_u2 = r->UInt();
	diffuse = String::ToWString(r->String());

	LoadInstancing(tem);

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
}

void MeshInstancingEditor::On()
{
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

Collider* MeshInstancingEditor::AddCollider(UINT index)
{
	Collider* collider = CollisionManager::Get()->CreateCollider();

	Transform* transform = meshes[index]->GetTransform();
	collider->GetTransform()->SetParent(transform);
	Matrix w;
	D3DXMatrixIdentity(&w);
	collider->GetTransform()->LocalWorld(w);

	colliders[index] = collider;
	return collider;
}


void MeshInstancingEditor::AddInstance()
{
	int index = meshes.size();
	meshes.push_back(meshInstancing->AddInstance());
	colliders.push_back(nullptr);

	Select(index);
}


#pragma region Create Instancing

void MeshInstancingEditor::MakingInstancing()
{
	if (ImGui::Combo("Mesh", &item, meshItems))
	{
		switch (item)
		{
		case 0: /* Cube */ break;
		case 1: /* Cylinder */ _f1 = 0.5f; _f2 = 3.0f; _u1 = 20; _u2 = 20; break;
		case 2: /* Plane */ _f1 = 2.5f; _f2 = 2.5f; break;
		case 3: /* Quad */ break;
		case 4: /* Sphere */ _f1 = 0.5f; _u1 = 20; _u2 = 20; break;
		}
	}

	switch (item)
	{
	case 0:
	{
		if (ImGui::Button("Create Cube"))
		{
			SetInstancing(new MeshInstancing(shader, new MeshCube()));
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
			SetInstancing(new MeshInstancing(shader, new MeshCylinder(_f1, _f2, _u1, _u2)));
			AddInstance();
		}
	}break;

	case 2:
	{
		ImGui::InputFloat("OffsetU", &_f1);
		ImGui::InputFloat("OffsetV", &_f2);
		if (ImGui::Button("Create Plane"))
		{
			SetInstancing(new MeshInstancing(shader, new MeshPlane(_f1, _f2)));
			AddInstance();
		}
	}break;

	case 3:
	{
		if (ImGui::Button("Create Quad"))
		{
			SetInstancing(new MeshInstancing(shader, new MeshQuad()));
			AddInstance();
		}
	}break;

	case 4:
	{
		ImGui::InputFloat("Radius", &_f1);
		ImGui::InputInt("StackCount", (int*)&_u1);
		ImGui::InputInt("SliceCount", (int*)&_u2);
		if (ImGui::Button("Create Sphere"))
			SetInstancing(new MeshInstancing(shader, new MeshSphere(_f1, _u1, _u2)));
	}break;

	}

	if (ImGui::Button(("Diffuse : " + String::ToString(diffuse)).c_str()))
	{
		Path::OpenFileDialog(L"",
			L"Texture File\0*.PNG\0*.TGA\0*.JPG", URI::Textures,
			[&](wstring r)
		{
			diffuse = Path::GetFileName(r);
		},
			WinDesc::GetHandle()
			);
	}

}

void MeshInstancingEditor::LoadInstancing(UINT item)
{
	switch (item)
	{
	case 0: SetInstancing(new MeshInstancing(shader, new MeshCube())); break;
	case 1: SetInstancing(new MeshInstancing(shader, new MeshCylinder(_f1, _f2, _u1, _u2))); break;
	case 2: SetInstancing(new MeshInstancing(shader, new MeshPlane(_f1, _f2))); break;
	case 3: SetInstancing(new MeshInstancing(shader, new MeshQuad())); break;
	case 4: SetInstancing(new MeshInstancing(shader, new MeshSphere(_f1, _u1, _u2))); break;
	}
}

void MeshInstancingEditor::SetInstancing(MeshInstancing * value)
{
	meshInstancing = value;
	if (diffuse.size() != 0)
		value->GetRenderer()->GetDefaultMaterial()->DiffuseMap(diffuse);
}

#pragma endregion

