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
	ModelSkinnedInstance* GetInstance(UINT instance) { return instances[instance]; }
	void RemoveInstance(ModelSkinnedInstance* value);

	void UpdateTransforms();
	void UpdateColors();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	void SetColor(UINT instance, const Color& color);
	void GetAttachBones(UINT instace, Matrix * matrix);
	UINT BoneCount() const { return boneCount; }

private:
	void ApplyModel(Shader* shader);

private: // compute ����
	class ModelComputeAnimInst* compute;

private: // instance ����
	vector<ModelSkinnedInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MODEL_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

	Color colors[MODEL_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceColorBuffer;

private: // render ����
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