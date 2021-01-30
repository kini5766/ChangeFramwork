#pragma once

#include "../Main/IObjectEditor.h"

class MeshInstancingEditor : public IObjectEditor
{
public:
	MeshInstancingEditor();
	~MeshInstancingEditor();

public:
	// IObjectEditor��(��) ���� ��ӵ�
	void ImGuiRender() override;
	void Update() override;
	void Render() override;
	void Save(class BinaryWriter * w) override;
	void Load(class BinaryReader * r) override;
	void On() override;
	void Off() override;

private:
	void AddInstance();
	void Select(int i);
	ColliderBox* AddCollider(UINT index);

	// create instancing
private:
	void MakingInstancing();
	void LoadInstancing(UINT item);
	void SetInstancing(MeshInstancing* value);

	// render
private:
	Shader* shader;
	vector<MeshInstance*> meshes;
	vector<ColliderBox*> colliders;
	MeshInstancing* meshInstancing = nullptr;

	// imgui
private:
	int selected = -1;
	int imguiItem = 0;

	// save load
private:
	class ComponentTransform* tImGui;
	class ComponentCollider* cImGui;
	class ComponentMaterial* mImGui;
	float _f1 = 1.0f;
	float _f2 = 1.0f;
	UINT _u1 = 1u;
	UINT _u2 = 1u;
	int meshType = -1;


private:
	const char* meshItems = {
		"Cube\0"
		"Cylinder\0"
		"Plane\0"
		"Quad\0"
		"Sphere\0"
	};
};