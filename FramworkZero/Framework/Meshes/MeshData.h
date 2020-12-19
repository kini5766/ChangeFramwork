#pragma once

struct SubMeshData
{
	UINT StartVertex;
	UINT VertexCount;
	UINT StartIndex;
	UINT IndexCount;
};

struct MeshBindPose
{
};

struct MeshData
{
public:
	// buffer values
	UINT VertexCount = 0;
	void* Vertices = nullptr;
	UINT Stride = 0;

	UINT IndexCount = 0;
	UINT* Indices = nullptr;

public:
	// delete
	void DeleteArray()
	{
		SafeDeleteArray(Vertices);  // �Ҹ��ڸ� ȣ������ ���� (delete void*)
		SafeDeleteArray(Indices);
	}

	virtual void SafeDeleteData()
	{
		//SafeDelete(PBind);
		DeleteArray();

		VertexCount = 0;
		Stride = 0;
		IndexCount = 0;
	}

public:
	// new
	template<typename T>
	T* NewVertices(UINT count);
	void NewIndices(UINT count);

	// copy
	template<typename T>
	void SetVertices(const T* vertices, UINT count);
	void SetIndices(const UINT* indeces, UINT count);

	// move
	void SetMoveVertices(void** vertices, UINT* stride, UINT* count);
	void SetMoveIndices(UINT** indeces, UINT* count);

public:
	// static
	template<typename T>
	static void Copy(MeshData* dst, const MeshData* src);
	static void Move(MeshData* dst, MeshData* src);
};


// new
template<typename T>
inline T * MeshData::NewVertices(UINT count)
{
	T* result = new T[count];
	VertexCount = count;
	Vertices = result;
	Stride = sizeof(T);
	return result;
}

inline void MeshData::NewIndices(UINT count)
{
	IndexCount = count;
	Indices = new UINT[count];
}

// copy
template<typename T>
inline void MeshData::SetVertices(const T * vertices, UINT count)
{
	NewVertices<T>(count);
	memcpy(Vertices, vertices, Stride * count);
}

inline void MeshData::SetIndices(const UINT* indeces, UINT count)
{
	NewIndices(count);
	memcpy(Indices, indeces, sizeof(UINT) * count);
}

// move
inline void MeshData::SetMoveVertices(void** vertices, UINT* stride, UINT* count)
{
	Vertices = (*vertices);
	(*vertices) = nullptr;
	Stride = (*stride);
	(*stride) = 0;
	VertexCount = (*count);
	(*count) = 0;
}

inline void MeshData::SetMoveIndices(UINT** indeces, UINT* count)
{
	Indices = (*indeces);
	IndexCount = (*count);
	(*indeces) = nullptr;
	(*count) = 0;
}

//static
template<typename T>
inline void MeshData::Copy(MeshData * dst, const MeshData * src)
{
	UINT stride = sizeof(T);
	if (stride != src->Stride) assert(false);  // �߸� �� ī��

	dst->SetVertices<T>((const T *)src->Vertices, src->VertexCount);
	dst->SetIndices(src->Indices, src->IndexCount);
}

inline void MeshData::Move(MeshData* dst, MeshData* src)
{
	dst->SetMoveVertices(&src->Vertices, &src->Stride, &src->VertexCount);
	dst->SetMoveIndices(&src->Indices, &src->IndexCount);
}
