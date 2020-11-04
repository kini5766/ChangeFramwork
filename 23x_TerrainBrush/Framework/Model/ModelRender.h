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
	Model* GetModel() { return model; }
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);

public:
	Transform* GetTransform() { return transform; }
	void Pass(UINT value);
	void UpdateTransform(ModelBone* bone = nullptr, const Matrix& matrix = Matrix());

private:
	void UpdateBones(ModelBone* bone, const Matrix& matrix);

private:
	Shader* shader;
	Model* model;

	Transform* transform;

	Matrix* boneTransforms = nullptr;
	UINT boneCount;
};