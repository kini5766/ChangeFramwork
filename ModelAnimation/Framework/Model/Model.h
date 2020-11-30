#pragma once

// 이제 필요없음
//#define MAX_MODEL_TRANSFORMS 250
//#define MAX_MODEL_KEYFRAMES 500

// 모델 관련 파일 불러오기

class ModelBone;
class ModelMesh;
struct ClipData;

class Model
{
public:
	typedef VertexTextureNormalTangentBlend ModelVertex;
	 
public:
	Model();
	~Model();

public:
	// transform : 보정값
	void Attach(Shader* shader, Model* model, int parentBoneIndex, Transform* offset = nullptr);

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

public:
	// Material
	UINT MaterialCount() { return materials.size(); }
	vector<ModelMaterial*>& Materials() { return materials; }
	ModelMaterial* MaterialByIndex(UINT index) { return materials[index]; }
	ModelMaterial* MaterialByName(wstring name);

public:
	// ModelBone
	UINT BoneCount() { return bones.size(); }
	vector<ModelBone*>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

public:
	// ModelMesh
	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh*>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }

public:
	// Animation Clip
	UINT ClipCount() { return clips.size(); }
	vector<ClipData*>& Clips() { return clips; }
	ClipData* ClipByIndex(UINT index) { return clips[index]; }
	ClipData* ClipByName(wstring name);


private:
	void BindBone();  // 본 부모 넣기
	void BindMesh();  // 메쉬에 본 넣기

private:
	ModelBone* root;
	vector<ModelMaterial*> materials;

	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;
	vector<ClipData*> clips;
};

// 3대요소
// 1. 머테리얼 (.material)
// 2. 뼈 (트랜스폼 부모자식 관계)
// 3. 메쉬 (.mesh)

//리깅 스키닝 