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
	void UpdateBoneTracking();

public:
	ModelSkinnedInstance* AddInstance();
	ModelSkinnedInstance* GetInstance(UINT instance) { return instances[instance]; }
	void RemoveInstance(ModelSkinnedInstance* value);

	void UpdateTransforms();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	BlendDesc* GetAnimationDesc(UINT index);
	const ModelData* GetModel() const { return data; }

private:
	void ApplyModel(Shader* shader);

private: // compute 관련
	class ModelComputeAnimInst* compute;
	// 스키닝 없는 모델
	TextureBuffer* computeOutputSrvBuffer;

private: // instance 관련
	vector<ModelSkinnedInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MODEL_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

private: // render 관련
	ModelData* data;
	SkinnedMeshRenderer* renderer;
	PerFrame* perframe;
	ID3D11Texture2D* invBindPose;
	ID3D11ShaderResourceView* srvInvBindPose;
};


// --
// ModelInstance
// --

class ModelSkinnedInstance
{
public:
	ModelSkinnedInstance(ModelSkinnedInstancing* perent, UINT id);
	~ModelSkinnedInstance();

public:
	void Update();

public:
	void Release() { perent->RemoveInstance(this); }
	ModelSkinnedInstancing* Perent() { return perent; }
	UINT Id() { return id; }

public:
	Transform* GetTransform() { return transform; }
	class ModelAnimation* GetAnimation();

private:
	ModelSkinnedInstancing* perent;
	class ModelAnimation* animation;
	Transform* transform;
	UINT id;

public:
	void UpdateBoneTracking(Matrix* tracking);
	Matrix* GetAttachBones() { return bones; }
	Matrix GetAttachBone(UINT boneIndex);

private:
	bool bBoneTracking = false;
	UINT boneCount = 0;
	Matrix* bones;
};