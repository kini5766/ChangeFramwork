#pragma once

// ------------------------------------------------------------------------------------------------
// ModelBone
// ------------------------------------------------------------------------------------------------

struct ModelBoneData
{
	int Index;
	wstring Name;

	int ParentIndex;
	Matrix Transform;
};

class ModelBone
{
public:
	ModelBone();
	~ModelBone();

public:
	void Render();

public:
	ModelBoneData* Data() { return data; }
	TransformData* GetTransform() { return transform; }
	void ReplaceMatrixGetter(Matrix* pMatrixGetter) { transform->ReplaceMatrixGetter(pMatrixGetter); }
	ID3D11Buffer* Buffer() { return boneBuffer->Buffer(); }
	void BindBone(ModelBone* _parent);

private:
	ModelBoneData* data;
	TransformData* transform;

	ModelBone* parent = nullptr;
	vector<ModelBone*> childs;

	ConstantBuffer* boneBuffer = nullptr;

private:
	struct BoneDesc
	{
		Matrix Transform;
		UINT BoneIndex;
		float Padding[3];
	}boneDesc;
};


// ------------------------------------------------------------------------------------------------
// ModelMesh
// ------------------------------------------------------------------------------------------------

class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

public:
	void Update();
	void Render();

public:
	void Data(const ModelMeshData* value);
	const ModelMeshData* Data() { return data; }

	void SetMaterial(ModelMaterial* mat);
	ModelMaterial* GetMaterial() { return material; }
	void Pass(UINT value) { pass = value; }
	int BoneIndex() { return data->PBind->BoneIndex; }
	void BindBone(ModelBone* value) { data->PBind->PBone = value; }

private:
	ModelMeshData* data;

	Shader* shader = nullptr;
	ModelMaterial* material = nullptr;

	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	ID3DX11EffectConstantBuffer* sBoneBuffer;
};
