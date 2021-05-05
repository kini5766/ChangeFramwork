#pragma once

struct ModelInstanceDesc;
class ModelInstance;

class ModelInstancing
{
public:
	ModelInstancing(const ModelDesc& desc);
	virtual ~ModelInstancing();

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
	UINT GetInstanceCount() { return instances.size(); }
	void RemoveInstance(ModelInstance* value);

	// Getter, Setter
public:
	ModelData* GetModel() { return renderer->GetModel(); }
	bool EnableAnim() const { return enableAnim; }
	AnimData* GetAnimData() { return animData; }
	void SetEnablePreEnvCube(bool value) { envCubeCaster->SetActive(value); }

private:  // PreRender
	void PreRender_Depth();
	void PreRender_EnvCube();
	ShadowCaster* shadowCaster;
	EnvCubeCaster* envCubeCaster;


private: // model 관련
	ModelRenderer* renderer;
	bool enableAnim;
	AnimData* animData;

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
	struct BlendDesc* Blend = nullptr;
	const AnimData* AnimData = nullptr;

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
	class Animator* GetAnimator();
	void UpdateBoneTracking(Matrix* tracking);
	Matrix* GetAttachBones() { return bones; }
	Matrix GetAttachBone(UINT boneIndex);

private:
	Transform* transform;

	bool enableAnim;
	class Animator* animator = nullptr;
	BlendDesc* blendDesc;

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


