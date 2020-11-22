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

	// Attach�� �� �߰�
	void AttachItem(Model * item, int parentBoneIndex, Transform * offset = nullptr);
	void SetActiveItem(UINT index, bool active);
	void OnlyOrigin();

public:
	// �޽� ������ ���� �Լ�
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
	vector<class ModelMesh*> renderers;
	class ModelClipMap* clipMap = nullptr;

private:
	Shader* shader;
	MaterialGroup* materials;
	Transform* transform;
	PerFrameBuffer* perFrame;
	class ModelAnimation* animation;

private:
	struct AttachedItem
	{
		Model* Model = nullptr;
		vector<ModelMesh*> Renderers;

		ModelClipMap* ClipMap = nullptr;

		bool IsActiveSelf = true;
	};

	vector<AttachedItem*> attachedItems;
};
