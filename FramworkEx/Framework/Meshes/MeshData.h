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

	// bind
	BindValue* PBind = nullptr;


public:
	MeshData() {}
	~MeshData()
	{
		SafeDelete(PBind);
		SafeDeleteArray(Vertices);  // 소멸자를 호출하지 않음 (delete[] void*)
		SafeDeleteArray(Indices);
	}

	template<typename T>T* NewVertices(UINT count) { T* result = new T[count];  VertexCount = count; Vertices = result; Stride = sizeof(T); return result; }
	void NewIndices(UINT count) { IndexCount = count; Indices = new UINT[count]; }

	void NewBindValue() { PBind = new BindValue(); }


private:
	MeshData(MeshData& rhs) { Move(*this, rhs); }
	void operator=(MeshData& rhs) { Move(*this, rhs); }

public:
	static void Move(MeshData& dst, MeshData& src)
	{
		dst.Vertices = src.Vertices; src.Vertices = nullptr;
		dst.VertexCount = src.VertexCount; src.VertexCount = 0;
		dst.Stride = src.Stride; src.Stride = 0;

		dst.Indices = src.Indices; src.Indices = nullptr;
		dst.IndexCount = src.IndexCount; src.IndexCount = 0;

		dst.PBind = src.PBind; src.PBind = nullptr;
	}

protected:
	template<typename T>T* NewCopyVertices(vector<T>& src, UINT count)
	{
		T* result = NewVertices<T>(count);
		copy(src.begin(), src.end(), stdext::checked_array_iterator<T*>(result, VertexCount));
		Vertices = result;
		return result;
	}
};