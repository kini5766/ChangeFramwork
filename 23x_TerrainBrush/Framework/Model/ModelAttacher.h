#pragma once

class ModelAttacher
{
public:
	ModelAttacher(Shader* shader);
	~ModelAttacher();

public:
	void Update();
	void Render();

public:
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f);

	// Attach할 모델 추가
	void AttachItem(Model * item, int parentBoneIndex, Transform * offset = nullptr);
	void SetActiveItem(UINT index, bool active);
	void OnlyOrigin();

public:
	// 메쉬 생성을 위한 함수
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);
	void ApplyOriginModel();

public:
	Model* GetOriginalModel() { return model; }
	Transform* GetTransform() { return transform; }
	void Pass(UINT value);

	float GetClipLength(UINT clip);
	float GetClipRunTime();

private:
	Model* model;
	vector<class ModelMeshClipMap*> renderers;

	Shader* shader;
	Transform* transform;

	class ModelAnimation* animation = nullptr;
	class ModelClipMap* clipMap = nullptr;

private:
	struct AttachedItem
	{
		Model* Model = nullptr;
		vector<ModelMeshClipMap*> Renderers;

		ModelClipMap* ClipMap = nullptr;

		bool IsActiveSelf = true;
	};

	vector<AttachedItem*> attachedItems;
};
