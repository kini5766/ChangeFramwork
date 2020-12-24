#pragma once

#include "ModelDesc.h"


class ModelData
{
public:
	ModelData();
	~ModelData();

public:
	UINT BoneCount() const { return bones.size(); }
	vector<ModelBoneData*>& Bones() { return bones; }
	ModelBoneData* BoneByIndex(UINT index) { return bones[index]; }

	UINT MeshCount() const { return meshes.size(); }
	vector<ModelMeshData*>& Meshes() { return meshes; }
	ModelMeshData* MeshByIndex(UINT index) { return meshes[index]; }

	UINT MaterialCount() const { return materials.size(); }
	vector<Material*>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	UINT MaterialIndexByName(wstring name);

	UINT ClipCount() const { return clips.size(); }
	vector<ModelClipData*>& Clips() { return clips; }
	ModelClipData* ClipByIndex(UINT index) { return clips[index]; }

public:
	// 읽기 순서 지켜주세요
	// 1. ReadMaterial
	// 2. ReadMesh
	// 3. ReadClip

	// .material 파일
	void ReadMaterial(wstring file);
	// .mesh 파일
	void ReadMesh(wstring file);
	// .clip 파일
	void ReadClip(wstring file);

private:
	vector<Material*> materials;
	vector<ModelMeshData*> meshes;
	vector<ModelBoneData*> bones;
	vector<ModelClipData*> clips;
};

