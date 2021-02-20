#pragma once

#include "ModelDesc.h"

class ModelData
{
public:
	ModelData();
	~ModelData();

public:
	SkinnedMeshData* GetRawData() { return meshRaw; }

	UINT BoneCount() const { return bones.size(); }
	const ModelBoneData*const* Bones() const { return bones.data(); }

	vector<Material*>& GetMaterials() { return meshRaw->Materials; }
	UINT MaterialIndexByName(wstring name);

	UINT ClipCount() const { return clips.size(); }
	const ModelClipData*const* Clips() const { return clips.data(); }

public:
	// .material ����
	void ReadMaterial(wstring file);
	// .mesh ����
	// ���͸����� ���� �о���մϴ�. (���͸���� ����޽��� ����)
	void ReadMesh(wstring file);
	// .clip ����
	void ReadClip(wstring file);

private:
	SkinnedMeshData* meshRaw;

	vector<ModelMeshData*> meshes;
	vector<ModelBoneData*> bones;
	vector<ModelClipData*> clips;
};
