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
	void ApplyModel();

private:
	Model* model;
	vector<class ModelMesh*> renderers;

	Shader* shader;
	Transform* transform;
	PerFrameBuffer* perFrame;
	MaterialGroup* materials;

	Matrix* boneTransforms = nullptr;
	UINT boneCount;
};