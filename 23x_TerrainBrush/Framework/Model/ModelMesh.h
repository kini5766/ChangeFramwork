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
public: friend class Model;

private:
	ModelMesh();
	~ModelMesh();

	void Binding(Model* model);

public:
	void Pass(UINT value) { pass = value; }
	void SetShader(Shader* value);

	void Update();
	void Render();

	wstring Name() { return name; }

	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }

	// 메쉬 위치 셋팅
	void BoneTransform(Matrix* value);
	// 모델의 최종 위치 셋팅
	void SetTransform(Transform* value);
	//TransformsSRV
	void TransformsSRV(ID3D11ShaderResourceView* value);

private:
	wstring name;

	Shader* shader;
	UINT pass = 0;

	Transform* transform = nullptr;
	PerFrame* perFrame = nullptr;

	wstring materialName = L"";
	Material* material = nullptr;

	int boneIndex;
	class ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;

	ID3D11ShaderResourceView* transformsSRV = nullptr;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV = nullptr;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

private:
	struct BoneDesc
	{
		Matrix Transform;
		UINT BoneIndex;
		float Padding[3];
	}  boneDesc;
};