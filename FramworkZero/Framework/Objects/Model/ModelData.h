#pragma once

#include "ModelDesc.h"


class ModelData
{
public:
	ModelData();
	~ModelData();

public:
	UINT BoneCount() const { return bones.size(); }
	ModelBoneData* BoneByIndex(UINT index) { return bones[index]; }

	UINT MeshCount() const { return meshes.size(); }
	ModelMeshData* MeshByIndex(UINT index) { return meshes[index]; }

	vector<Material*>& Materials() { return materials; }
	UINT MaterialIndexByName(wstring name);

	UINT ClipCount() const { return clips.size(); }
	vector<ModelClipData*>& Clips() { return clips; }
	const ModelClipData* ClipByIndex(UINT index) const { return clips[index]; }

public:
	// �б� ���� �����ּ���
	// 1. ReadMaterial
	// 2. ReadMesh
	// 3. ReadClip

	// .material ����
	void ReadMaterial(wstring file);
	// .mesh ����
	void ReadMesh(wstring file);
	// .clip ����
	void ReadClip(wstring file);

private:
	vector<Material*> materials;
	vector<ModelMeshData*> meshes;
	vector<ModelBoneData*> bones;
	vector<ModelClipData*> clips;
};

