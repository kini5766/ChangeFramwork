#pragma once

// ���� �ʿ����
//#define MAX_MODEL_TRANSFORMS 250
//#define MAX_MODEL_KEYFRAMES 500

// �� ���� ���� �ҷ�����

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
	// transform : ������
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
	void BindBone();  // �� �θ� �ֱ�
	void BindMesh();  // �޽��� �� �ֱ�

private:
	ModelBone* root;
	vector<ModelMaterial*> materials;

	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;
	vector<ClipData*> clips;
};

// 3����
// 1. ���׸��� (.material)
// 2. �� (Ʈ������ �θ��ڽ� ����)
// 3. �޽� (.mesh)

//���� ��Ű�� 