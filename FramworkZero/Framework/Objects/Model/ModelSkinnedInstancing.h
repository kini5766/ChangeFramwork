#pragma once

class ModelSkinnedInstance;

class ModelSkinnedInstancing
{
public:
	ModelSkinnedInstancing(Shader* shader, const ModelDesc& desc);
	~ModelSkinnedInstancing();

public:
	void Update();
	void Render();

public:
	ModelSkinnedInstance* AddInstance();
	void RemoveInstance(ModelSkinnedInstance* value);

	void UpdateTransforms();
	void UpdateColors();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	void SetColor(UINT instance, const Color& color);


private:
	void ApplyModel(Shader* shader);
	void CreateCompute();

private: // animation ����
	class ModelAnimationInstancing* animation;

private: // instance ����
	vector<ModelSkinnedInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

	Color colors[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceColorBuffer;

private: // render ����
	ModelData* data;
	SkinnedMeshRenderer* renderer;
	Transform* transform;

	Matrix world;
	UINT boneCount;
	// (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	ID3D11ShaderResourceView* srvLocalBones = nullptr;

	ShaderSetter* computeShader;
	// In : ��World
	// In : (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	// In : inv ��, �� �θ� �ε�����
	StructuredBuffer* computeBoneDescBuffer;
	// Out : ���� ��*�ν��Ͻ� texture
	TextureBuffer* computeOutputBoneBuffer;
	// Out : Skinned�����ų ��*�ν���Ʈ texture
	TextureBuffer* computeOutputSrvBuffer;

private:
	struct BoneDesc
	{
		// �� �θ� �ε���
		int Parent;
		// inv ��
		Matrix InvBone;
		// ���׺� ���� �� ���� ��ų ��
		Matrix DefaultBone;
	}* boneDesc;
};


// --
// MeshInstance
// --

class ModelSkinnedInstance
{
public:
	ModelSkinnedInstance(ModelSkinnedInstancing* perent, UINT id)
		: perent(perent), id(id)
	{
		transform = new Transform();
	}
	~ModelSkinnedInstance()
	{
		SafeDelete(transform);
	}

public:
	ModelSkinnedInstancing* Perent() { return perent; }
	Transform* GetTransform() { return transform; }
	UINT Id() { return id; }
	void Release() { perent->RemoveInstance(this); }

private:
	ModelSkinnedInstancing* perent;
	Transform* transform;
	UINT id;
};