#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

public:
	void Update();
	void Render();

public:
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f);

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

public:
	Model* GetModel() { return model; }
	Transform* GetTransform() { return transform; }
	void Pass(UINT value);

private:
	void Apply();

private:
	Model* model;

	Shader* shader;
	Transform* transform;
	PerFrame* perFrame;
	class ModelAnimationEx* animation;
};
