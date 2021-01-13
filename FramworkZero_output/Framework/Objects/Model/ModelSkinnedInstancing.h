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
	void UpdateColors();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	void SetColor(UINT instance, const Color& color);
	//void GetAttachBones(UINT instace, Matrix * matrix);
	BlendDesc* GetAnimationDesc(UINT index);
	const ModelData* GetModel() const { return data; }

private:
	void ApplyModel(Shader* shader);

private: // compute 관련
	class ModelComputeAnimInst* compute;

private: // instance 관련
	vector<ModelSkinnedInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MODEL_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

	Color colors[MODEL_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceColorBuffer;

private: // render 관련
	ModelData* data;
	SkinnedMeshRenderer* renderer;
	Transform* transform;
	PerFrame* perframe;
	Matrix world;
	UINT boneCount;
	Texture2D* invBindPose;
};


// --
// MeshInstance
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
	class ModelAnimation* GetAnimation() { return animation; }

private:
	ModelSkinnedInstancing* perent;
	class ModelAnimation* animation;
	Transform* transform;
	UINT id;

public:
	void UpdateBoneTracking(Matrix* tracking);
	Matrix* GetAttachBones() { return bones; }
	Matrix GetAttachBone(UINT instace);

private:
	bool bBoneTracking = false;
	UINT boneCount = 0;
	Matrix* bones;
};