#pragma once

#include "SceneEditor/Component/ComponentTransform.h"
#include "SceneEditor/Component/ComponentCollider.h"
#include "SceneEditor/Component/ComponentMaterial.h"

class MeshInstancingDesc
{
public:
	ComponentTransform* TImGui;
	ComponentCollider* CImGui;
	ComponentMaterial* MImGui;
	float _F1 = 1.0f;
	float _F2 = 1.0f;
	UINT _U1 = 1u;
	UINT _U2 = 1u;
	int MeshType = -1;

public:
	MeshInstancing* Mesh = nullptr;
	vector<MeshInstance*> Meshes;
	vector<ColliderBox*> Colliders;

public:
	MeshInstancingDesc();
	~MeshInstancingDesc();

public:
	void Update();
	void Render();

public:
	void AddInstance();
	void SetInstancing();
	ColliderBox* AddCollider(UINT index);
	MeshInstancing* CreateInstancing(UINT type);
};