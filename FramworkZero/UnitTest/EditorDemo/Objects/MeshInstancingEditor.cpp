#include "stdafx.h"
#include "MeshInstancingEditor.h"

#include "Utilities/BinaryFile.h"

MeshInstancingEditor::MeshInstancingEditor()
{
	shader = Shader::Load(L"01_Material.fxo");
}

MeshInstancingEditor::~MeshInstancingEditor()
{
	SafeDelete(meshInstancing);
	SafeRelease(shader);
}

void MeshInstancingEditor::ImGuiRender()
{
	if (meshInstancing == nullptr)
	{
		MakingMesh();
		return;
	}

	if (ImGui::Button("+ Add Instance"))
	{

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
	w->Float(_f1);
	w->Float(_f2);
	w->UInt(_u1);
	w->UInt(_u2);
}

void MeshInstancingEditor::Load(BinaryReader * r)
{
	int tem = r->Int();
	if (tem == -1) return;

	_f1 = r->Float();
	_f2 = r->Float();
	_u1 = r->UInt();
	_u2 = r->UInt();
}

void MeshInstancingEditor::On()
{
}

void MeshInstancingEditor::Off()
{
	Debug::Gizmo->SetTransform(nullptr);
}


void MeshInstancingEditor::MakingMesh()
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
			meshInstancing = new MeshInstancing(shader, new MeshCube());
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
			meshInstancing = new MeshInstancing(shader, new MeshCylinder(_f1, _f2, _u1, _u2));
		}
	}break;

	case 2:
	{
		ImGui::InputFloat("OffsetU", &_f1);
		ImGui::InputFloat("OffsetV", &_f2);
		if (ImGui::Button("Create Plane"))
		{
			meshInstancing = new MeshInstancing(shader, new MeshPlane(_f1, _f2));
		}
	}break;

	case 3:
	{
		if (ImGui::Button("Create Quad"))
		{
			meshInstancing = new MeshInstancing(shader, new MeshQuad());
		}
	}break;

	case 4:
	{
		ImGui::InputFloat("Radius", &_f1);
		ImGui::InputInt("StackCount", (int*)&_u1);
		ImGui::InputInt("SliceCount", (int*)&_u2);
		if (ImGui::Button("Create Sphere"))
		{
			meshInstancing = new MeshInstancing(shader, new MeshSphere(_f1, _u1, _u2));
		}
	}break;

	}

}

void MeshInstancingEditor::CreateMesh(UINT i)
{
	switch (i)
	{
	case 0: new MeshInstancing(shader, new MeshCube()); break;
	case 1: new MeshInstancing(shader, new MeshCylinder(_f1, _f2, _u1, _u2)); break;
	case 2: new MeshInstancing(shader, new MeshPlane(_f1, _f2)); break;
	case 3: new MeshInstancing(shader, new MeshQuad()); break;
	case 4: new MeshInstancing(shader, new MeshSphere(_f1, _u1, _u2)); break;
	}
}
