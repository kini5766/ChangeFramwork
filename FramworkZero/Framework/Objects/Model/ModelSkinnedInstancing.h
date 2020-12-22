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

private: // animation 관련
	class ModelAnimationInstancing* animation;

private: // instance 관련
	vector<ModelSkinnedInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

	Color colors[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceColorBuffer;

private: // render 관련
	ModelData* data;
	SkinnedMeshRenderer* renderer;
	Transform* transform;

	Matrix world;
	UINT boneCount;
	// (애니메이션out) 로컬 본*인스턴스 texture
	ID3D11ShaderResourceView* srvLocalBones = nullptr;

	ShaderSetter* computeShader;
	// In : 모델World
	// In : (애니메이션out) 로컬 본*인스턴스 texture
	// In : inv 본, 본 부모 인덱스들
	StructuredBuffer* computeBoneDescBuffer;
	// Out : 로직 본*인스턴스 texture
	TextureBuffer* computeOutputBoneBuffer;
	// Out : Skinned적용시킬 본*인스턴트 texture
	TextureBuffer* computeOutputSrvBuffer;

private:
	struct BoneDesc
	{
		// 본 부모 인덱스
		int Parent;
		// inv 본
		Matrix InvBone;
		// 에닝본 없을 시 적용 시킬 본
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