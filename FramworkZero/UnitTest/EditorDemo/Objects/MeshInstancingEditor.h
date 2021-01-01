#pragma once

#include "../IObjectEditor.h"

class MeshInstancingEditor : public IObjectEditor
{
public:
	MeshInstancingEditor();
	~MeshInstancingEditor();

public:
	// IObjectEditor을(를) 통해 상속됨
	void ImGuiRender() override;
	void Update() override;
	void Render() override;
	void Save(class BinaryWriter * w) override;
	void Load(class BinaryReader * r) override;
	void On() override;
	void Off() override;

private:
	void MakingMesh();
	void CreateMesh(UINT i);

private:
	Shader* shader;
	MeshInstancing* meshInstancing = nullptr;
	vector<MeshInstance*> meshes;
	vector<Collider*> colliders;

	float _f1 = 1.0f;
	float _f2 = 1.0f;
	UINT _u1 = 1u;
	UINT _u2 = 1u;

	int item = 0;
	const char* meshItems = {
		"Cube\0"
		"Cylinder\0"
		"Plane\0"
		"Quad\0"
		"Sphere\0"
	};
};