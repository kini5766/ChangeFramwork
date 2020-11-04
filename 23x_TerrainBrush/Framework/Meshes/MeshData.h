#pragma once

struct BoneWeight
{
	Vector4 BlendIndices = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector4 BlendWeights = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct BindValue
{
	int BoneIndex = -2;
	class ModelBone* PBone = nullptr;

	wstring Name = L"";
	wstring MaterialName = L"";
};


// 현재 사용하지 않지만 추후 사용할 계획
struct MeshData
{
	// buffer values
	UINT VertexCount = 0;
	Vector3* Positions = nullptr;
	Vector2* Uvs = nullptr;
	Vector3* Normals = nullptr;
	Vector3* Tangents = nullptr;
	BoneWeight* BoneWeights = nullptr;

	UINT IndexCount = 0;
	UINT* Indices = nullptr;

	BindValue* PBind = nullptr;


	~MeshData()
	{
		SafeDelete(PBind);
		SafeDeleteArray(BoneWeights);
		SafeDeleteArray(Tangents);
		SafeDeleteArray(Uvs);
		SafeDeleteArray(Normals);
		SafeDeleteArray(Positions);
		SafeDeleteArray(Indices);
	}

	void NewPositions() { Positions = new Vector3[VertexCount]; }
	void NewUvs() { Uvs = new Vector2[VertexCount]; }
	void NewNormals() { Normals = new Vector3[VertexCount]; }
	void NewTangents() { Tangents = new Vector3[VertexCount]; }
	void NewBoneWeights() { BoneWeights = new BoneWeight[VertexCount]; }

	void NewIndices(UINT count) { IndexCount = count; Indices = new UINT[IndexCount]; }

	void NewBindValue() { PBind = new BindValue(); }
};
