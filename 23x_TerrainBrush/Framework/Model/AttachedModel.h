#pragma once

class AttachedModel
{
public:
	AttachedModel(Shader* shader);
	~AttachedModel();

public:
	void UpdateTweening();
	void UpdateNoTweening();
	void Render();

public:
	void Pass(UINT value);
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f);
	void ChangeOriginal();
	void ChangeModel(UINT num);

	Transform* GetTransform() { return transform; }
	float GetClipLength(UINT clip);
	float GetClipRunTime();

public:
	// 메쉬 생성을 위한 함수
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);
	void ApplyOriginModel();
	// Attach할 모델들 추가
	void AddAttach(Shader * shader, Model * model, int parentBoneIndex, Transform * offset);
	// Attach된 모델들 추가
	void AddAttachedModel(Model * attachedModel);
	Model* GetOriginalModel() { return originalModel; }

private:
	void UpdateMesh();

private:
	Shader* shader;
	Transform* transform;
	class ClipAnimator* animator;

	Model* originalModel;
	class ClipTransformMap* originalMap;
	vector<Model*> attachedModels;
	vector<class ClipTransformMap*> attachedMaps;

private:
	// 선택된 모델
	Model* selectedModel = nullptr;
};