#include "stdafx.h"
#include "MeshInstancingDesc.h"

MeshInstancingDesc::MeshInstancingDesc()
{
	Shader = Shader::Load(L"01_Mesh.fxo");

	TImGui = new ComponentTransform();
	CImGui = new ComponentCollider();
	MImGui = new ComponentMaterial();

	shadow = new ShadowCaster(Shader);
	shadow->SetFuncPreRender(bind(&MeshInstancingDesc::PreRender_Depth, this));
	shadow->SetShadow_Global();
}

MeshInstancingDesc::~MeshInstancingDesc()
{
	SafeDelete(shadow);

	SafeDelete(Mesh);

	SafeDelete(MImGui);
	SafeDelete(CImGui);
	SafeDelete(TImGui);

	SafeRelease(Shader);
}

void MeshInstancingDesc::Update()
{
	if (Mesh == nullptr)
		return;

	Mesh->Update();
}

void MeshInstancingDesc::Render()
{
	if (Mesh == nullptr)
		return;

	Mesh->Pass(5);
	Mesh->Render();

	for (ColliderBox* collider : Colliders)
		if (collider != nullptr)
			Debug::Box->RenderBox(collider->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
}

void MeshInstancingDesc::PreRender_Depth()
{
	if (Mesh == nullptr)
		return;

	Mesh->Pass(4);
	Mesh->Render();
}

void MeshInstancingDesc::AddInstance()
{
	Meshes.push_back(Mesh->AddInstance());
	Colliders.push_back(nullptr);
}

void MeshInstancingDesc::SetInstancing()
{
	SafeDelete(Mesh);
	Meshes.clear();
	Colliders.clear();

	Mesh = CreateInstancing(MeshType);
	if (Mesh == nullptr)
		return;

	Mesh->Pass(1);
	MImGui->Apply(Mesh->GetRenderer()->GetDefaultMaterial());
}

ColliderBox * MeshInstancingDesc::AddCollider(UINT index)
{
	ColliderBox* collider = CollisionManager::Get()->CreateCollider();

	Transform* transform = Meshes[index]->GetTransform();
	collider->GetTransform()->SetParent(transform);
	Matrix w;
	D3DXMatrixIdentity(&w);
	collider->GetTransform()->LocalWorld(w);

	Colliders[index] = collider;
	return collider;
}

MeshInstancing* MeshInstancingDesc::CreateInstancing(UINT type)
{
	switch (type)
	{
	case 0: return(new MeshInstancing(Shader, unique_ptr<MeshData>(new MeshCube()))); break;
	case 1: return(new MeshInstancing(Shader, unique_ptr<MeshData>(new MeshCylinder(_F1, _F2, _U1, _U2)))); break;
	case 2: return(new MeshInstancing(Shader, unique_ptr<MeshData>(new MeshPlane(_F1, _F2)))); break;
	case 3: return(new MeshInstancing(Shader, unique_ptr<MeshData>(new MeshQuad()))); break;
	case 4: return(new MeshInstancing(Shader, unique_ptr<MeshData>(new MeshSphere(_F1, _U1, _U2)))); break;
	default: return nullptr;
	}
}
