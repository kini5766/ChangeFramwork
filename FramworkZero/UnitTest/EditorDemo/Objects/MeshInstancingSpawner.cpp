#include "stdafx.h"
#include "MeshInstancingSpawner.h"

#include "Component/TransformLoader.h"
#include "Component/ColliderLoader.h"
#include "Component/MaterialLoader.h"

MeshInstancingSpawner::MeshInstancingSpawner()
{
	shader = Shader::Load(L"01_Mesh.fxo");
}

MeshInstancingSpawner::~MeshInstancingSpawner()
{
	SafeDelete(meshInstancing);
	SafeRelease(shader);
}

void MeshInstancingSpawner::Update()
{
	meshInstancing->Update();
}

void MeshInstancingSpawner::Render()
{
	meshInstancing->Render();
}


#pragma region IObjectSpawner

void MeshInstancingSpawner::Initialize(SceneValueUnit * scene)
{
	scene->Updater->Action = bind(&MeshInstancingSpawner::Update, this);
	scene->Updater->bDestroy = false;

	scene->Renderer->Action = bind(&MeshInstancingSpawner::Render, this);
	scene->Renderer->bDestroy = false;
}

bool MeshInstancingSpawner::Load(BinaryReader * r)
{
	TransformLoader tLoader;
	ColliderLoader cLoader;
	MaterialLoader mLoader;

	int imguiItem = r->Int();
	if (imguiItem == -1) return false;

	// meshInstancing
	_f1 = r->Float();
	_f2 = r->Float();
	_u1 = r->UInt();
	_u2 = r->UInt();

	mLoader.Load(r);

	meshInstancing = GetInstancing(imguiItem);

	// meshInstance
	UINT size = r->UInt();
	for (UINT i = 0; i < size; i++)
	{
		MeshInstance* mesh = meshInstancing->AddInstance();

		tLoader.Load(mesh->GetTransform(), r);

		bool hasCollider = r->Bool();
		if (hasCollider)
		{
			ColliderBox* collider;
			{
				collider = CollisionManager::Get()->CreateCollider();
				Transform* transform = mesh->GetTransform();
				collider->GetTransform()->SetParent(transform);
				Matrix w;
				D3DXMatrixIdentity(&w);
				collider->GetTransform()->LocalWorld(w);
			}
			cLoader.Load(collider, r);
		}
	}

	if (meshInstancing == nullptr)
		return false;

	meshInstancing->Pass(1);
	mLoader.Apply(meshInstancing->GetRenderer()->GetDefaultMaterial());

	meshInstancing->UpdateTransforms();
	return true;
}

MeshInstancing * MeshInstancingSpawner::GetInstancing(UINT item)
{
	switch (item)
	{
	case 0: return(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshCube()))); break;
	case 1: return(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshCylinder(_f1, _f2, _u1, _u2)))); break;
	case 2: return(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshPlane(_f1, _f2)))); break;
	case 3: return(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshQuad()))); break;
	case 4: return(new MeshInstancing(shader, unique_ptr<MeshData>(new MeshSphere(_f1, _u1, _u2)))); break;
	default: return nullptr;
	}
}

#pragma endregion

