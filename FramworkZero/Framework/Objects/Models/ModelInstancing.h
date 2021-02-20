#pragma once

struct ModelInstanceDesc;
class ModelInstance;

class ModelInstancing
{
public:
	ModelInstancing(const ModelDesc& desc);
	~ModelInstancing();

public:
	void Render();
	void Render_EnvCube();

	// Updates
public:
	void Update();
	void UpdateBoneTracking();
	void UpdateTransforms();

	// Instance
public:
	ModelInstance* AddInstance();
	ModelInstance* GetInstance(UINT instance) { return instances[instance]; }
	void RemoveInstance(ModelInstance* value);

	// Getter
public:
	ModelData* GetModel() { return renderer->GetModel(); }
	bool EnableAnim() const { return enableAnim; }


private:  // PreRender
	void PreRender_Depth();
	void PreRender_EnvCube();
	ShadowCaster* shadowCaster;
	EnvCubeCaster* envCubeCaster;


private: // model 관련
	ModelRenderer* renderer;
	bool enableAnim;

private: // instance 관련
	ModelInstanceDesc* instanceDesc;
	VertexBuffer* instanceBuffer;
	Matrix worlds[MODEL_INSTANCE_MAX_COUNT];
	vector<ModelInstance*> instances;
	vector<UINT> junkInstances;
	class ModelComputeAnimInst* compute;
};


// --
// ModelInstance
// --

struct ModelInstanceDesc
{
	UINT Id;
	UINT BoneCount;

	bool EnableAnim;
	UINT ClipCount = 0;
	const ModelClipData*const* Clips = nullptr;
	struct BlendDesc* Blend = nullptr;

	function<void(class ModelInstance*)> FuncRelease;
};

class ModelInstance
{
public:
	ModelInstance(const ModelInstanceDesc& desc);
	~ModelInstance();

	// Getter
public:
	Transform* GetTransform() { return transform; }

	// Animation
public:
	class AnimationAdapter* GetAnimAdapter();
	void UpdateBoneTracking(Matrix* tracking);
	Matrix* GetAttachBones() { return bones; }
	Matrix GetAttachBone(UINT boneIndex);

private:
	class AnimationAdapter* animation = nullptr;
	Transform* transform;
	bool bBoneTracking = false;
	UINT boneCount = 0;
	Matrix* bones;

	// Instancing
public:
	void Update();
	void Release() { funcRelease(this); }
	UINT Id() { return id; }

public:
	UINT id;
	function<void(class ModelInstance*)> funcRelease;
};