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


struct MeshData
{
	// buffer values
	UINT VertexCount = 0;
	void* Vertices = nullptr;
	UINT Stride = 0;

	UINT IndexCount = 0;
	UINT* Indices = nullptr;

	// Info
	//Vector3* Positions = nullptr;
	//Vector2* Uvs = nullptr;
	//Vector3* Normals = nullptr;
	//Vector3* Tangents = nullptr;
	//BoneWeight* BoneWeights = nullptr;

	// bind
	BindValue* PBind = nullptr;


	~MeshData()
	{
		SafeDelete(PBind);
		SafeDeleteArray(Vertices);  // 소멸자를 호출하지 않음 (delete void*)
		SafeDeleteArray(Indices);

		//SafeDeleteArray(BoneWeights);
		//SafeDeleteArray(Tangents);
		//SafeDeleteArray(Uvs);
		//SafeDeleteArray(Normals);
		//SafeDeleteArray(Positions);
	}

	void NewIndices(UINT count) { IndexCount = count; Indices = new UINT[IndexCount]; }

	void NewBindValue() { PBind = new BindValue(); }
};
