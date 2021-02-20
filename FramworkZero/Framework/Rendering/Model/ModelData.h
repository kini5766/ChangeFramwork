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
	// .material 파일
	void ReadMaterial(wstring file);
	// .mesh 파일
	// 매터리얼을 먼저 읽어야합니다. (매터리얼과 서브메쉬를 연결)
	void ReadMesh(wstring file);
	// .clip 파일
	void ReadClip(wstring file);

private:
	SkinnedMeshData* meshRaw;

	vector<ModelMeshData*> meshes;
	vector<ModelBoneData*> bones;
	vector<ModelClipData*> clips;
};
