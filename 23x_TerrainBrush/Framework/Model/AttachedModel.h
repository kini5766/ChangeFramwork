#pragma once
/*
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
	void ChangeModel(UINT num);  // 0은 원래대로

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
	Model* model;
	vector<ModelMeshBoneMap*> renderers;

	Shader* shader;
	Transform* transform;

	class AnimationClip* animator = nullptr;
	class ClipTransformMap* transfromMap = nullptr;

private:
	vector<vector<ModelMeshBoneMap*>> rendererArr2D;
	vector<ClipTransformMap*> attachedMaps;
	vector<bool> isActiveSelfs;
};
*/