#include "stdafx.h"
#include "MeshInstancingSpawner.h"

#include "EditorDemo/Component/TransformLoader.h"
#include "EditorDemo/Component/ColliderLoader.h"
#include "EditorDemo/Component/MaterialLoader.h"

MeshInstancingSpawner::MeshInstancingSpawner()
{
}

MeshInstancingSpawner::~MeshInstancingSpawner()
{
	SafeDelete(meshInstancing);
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

	InputValue input;

	input.Item = r->Int();
	if (input.Item == -1) return false;

	// meshInstancing
	input._F1 = r->Float();
	input._F2 = r->Float();
	input._U1 = r->UInt();
	input._U2 = r->UInt();

	mLoader.Load(r);

	meshInstancing = GetInstancing(input);

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

	//meshInstancing->Pass(1);
	mLoader.Apply(meshInstancing->GetMaterial());

	meshInstancing->UpdateTransforms();
	return true;
}

MeshInstancing * MeshInstancingSpawner::GetInstancing(InputValue& value)
{
	switch (value.Item)
	{
	case 0: return(new MeshInstancing(unique_ptr<MeshData>(new MeshCube()))); break;
	case 1: return(new MeshInstancing(unique_ptr<MeshData>(new MeshCylinder(value._F1, value._F2, value._U1, value._U2)))); break;
	case 2: return(new MeshInstancing(unique_ptr<MeshData>(new MeshPlane(value._F1, value._F2)))); break;
	case 3: return(new MeshInstancing(unique_ptr<MeshData>(new MeshQuad()))); break;
	case 4: return(new MeshInstancing(unique_ptr<MeshData>(new MeshSphere(value._F1, value._U1, value._U2)))); break;
	default: return nullptr;
	}
}

#pragma endregion

