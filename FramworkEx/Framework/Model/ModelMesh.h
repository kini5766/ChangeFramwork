#pragma once

// ------------------------------------------------------------------------------------------------
// ModelBone
// ------------------------------------------------------------------------------------------------

class ModelBone
{
public: friend class Model;

private:
	ModelBone();
	~ModelBone();

public:
	int Index() { return index; }
	int ParentIndex() { return parentIndex; }
	ModelBone* Parent() { return parent; }
	wstring Name() { return name; }

	Matrix& Transform() { return transform; }
	void Transform(const Matrix& matrix) { transform = matrix; }

	vector<ModelBone*>& Childs() { return childs; }

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> childs;
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
	void CreateBuffer(ModelMeshData* mesh);
	void SetMaterial(ModelMaterial* mat);
	void BindBone(ModelBone* bone);

	void Update();
	void Render();

	void Pass(UINT value) { pass = value; }
	UINT BoneIndex() { return boneDesc.BoneIndex; };

private:
	// 메쉬 위치 셋팅
	void BoneTransform(Matrix* value);
	void SetBoneIndex(UINT boneIndex);

private:
	ModelMeshData* mesh = nullptr;

	Shader* shader = nullptr;
	ModelMaterial* material = nullptr;

	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	ConstantBuffer* boneBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	struct BoneDesc
	{
		Matrix Transform;
		UINT BoneIndex;
		float Padding[3];
	}boneDesc;
};
