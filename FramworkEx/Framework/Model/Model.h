#pragma once

// ���� �ʿ����
//#define MAX_MODEL_TRANSFORMS 250
//#define MAX_MODEL_KEYFRAMES 500

// �� ���� ���� �ҷ�����

class ModelBone;
class ModelClip;

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
	void AttachWeakly(Model* model, int parentBoneIndex, Transform* offset = nullptr);
	void Copy(Model* out);

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

public:
	// Material
	UINT MaterialCount() { return materials.size(); }
	vector<Material*>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

public:
	// ModelBone
	UINT BoneCount() { return bones.size(); }
	vector<ModelBone*>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

public:
	// ModelMesh
	UINT MeshCount() { return meshes.size(); }
	vector<MeshData*>& Meshes() { return meshes; }
	MeshData* MeshByIndex(UINT index) { return meshes[index]; }
	MeshData* MeshByName(wstring name);

public:
	// Animation Clip
	UINT ClipCount() { return clips.size(); }
	vector<ModelClip*>& Clips() { return clips; }
	ModelClip* ClipByIndex(UINT index) { return clips[index]; }
	ModelClip* ClipByName(wstring name);


private:
	void BindBone();  // �� �θ� �ֱ�
	void BindMesh();  // �޽��� �� �ֱ�

private:
	ModelBone* root;
	vector<Material*> materials;

	vector<ModelBone*> bones;
	vector<MeshData*> meshes;
	vector<ModelClip*> clips;
};

// 3����
// 1. ���׸��� (.material)
// 2. �� (Ʈ������ �θ��ڽ� ����)
// 3. �޽� (.mesh)

//���� ��Ű�� 