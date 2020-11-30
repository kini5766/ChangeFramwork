#pragma once

class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

public:
	void Update();
	void Render();

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);

public:
	Model* GetModel() { return model; }
	Transform* GetTransform() { return transform; }
	void Pass(UINT value);

private:
	void ApplyModel();

private:
	Model* model;

	Shader* shader;
	Transform* transform;
	PerFrame* perFrame;
};