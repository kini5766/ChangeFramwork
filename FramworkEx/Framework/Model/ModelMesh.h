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
	void CreateBuffer(MeshData* mesh);
	void SetMaterial(Material* mat);
	void BindBone(ModelBone* bone);

	void Update();
	void Render();

	void Pass(UINT value) { pass = value; }
	UINT BoneIndex() { return boneDesc.BoneIndex; };

	// 메쉬 위치 셋팅
	void BoneTransform(Matrix* value);
	// 모델의 최종 위치 셋팅
	void SetTransform(Transform* value);

private:
	MeshData* mesh = nullptr;

	Shader* shader = nullptr;
	Material* material = nullptr;
	Transform* transform = nullptr;

	UINT pass = 0;
	PerFrame* perFrame = nullptr;

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


// ------------------------------------------------------------------------------------------------
// ModelMeshBaked
// ------------------------------------------------------------------------------------------------

class ModelMeshClipMap
{
public:
	ModelMeshClipMap();
	~ModelMeshClipMap();

public:
	void CreateBuffer(MeshData* mesh);
	void SetMaterial(Material* mat);

	void Update();
	void Render();

	void Pass(UINT value) { pass = value; }

	// 모델의 최종 위치 셋팅
	void SetTransform(Transform* value);

	//TransformsSRV
	void TransformsSRV(ID3D11ShaderResourceView* value, UINT boneIndex);

private:
	MeshData* mesh = nullptr;

	Shader* shader = nullptr;
	Material* material = nullptr;
	Transform* transform = nullptr;

	UINT pass = 0;
	PerFrame* perFrame = nullptr;

	ID3D11ShaderResourceView* transformsSRV = nullptr;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV = nullptr;

	VertexBuffer* vertexBuffer = nullptr;
	//UINT vertexCount = 0;
	//Model::ModelVertex* vertices = nullptr;

	IndexBuffer* indexBuffer = nullptr;
	//UINT indexCount = 0;
	//UINT* indices = nullptr;

	ConstantBuffer* boneBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	struct BoneDesc
	{
		Matrix Transform;
		UINT BoneIndex;
		float Padding[3];
	}boneDesc;
};