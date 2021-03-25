#pragma once


#pragma region VertexMesh

struct VertexMesh
{
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	Vector3 Tangent;

	VertexMesh()
	{
		Position = Vector3(0, 0, 0);
		Uv = Vector2(0, 0);
		Normal = Vector3(0, 0, 0);
		Tangent = Vector3(0, 0, 0);
	}

	VertexMesh(
		float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz
	)
	{
		Position = Vector3(x, y, z);
		Uv = Vector2(u, v);
		Normal = Vector3(nx, ny, nz);
		Tangent = Vector3(tx, ty, tz);
	}

	VertexMesh(
		Vector3 position,
		Vector2 uv,
		Vector3 normal,
		Vector3 tangent
	)
	{
		Position = position;
		Uv = uv;
		Normal = normal;
		Tangent = tangent;
	}
};

#pragma endregion


struct SubMeshData
{
	UINT MaterialIndex = 0;
	UINT StartVertex = 0;
	UINT VertexCount = 0;
	UINT StartIndex = 0;
	UINT IndexCount = 0;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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

	UINT SubMeshCount = 0;
	SubMeshData* SubMeshes = nullptr;

public:
	// delete
	void DeleteArray()
	{
		SafeDeleteArray(Vertices);  // 소멸자를 호출하지 않음 (delete void*)
		SafeDeleteArray(Indices);
	}

	virtual void SafeDeleteData()
	{
		//SafeDelete(PBind);
		DeleteArray();
		SafeDeleteArray(SubMeshes)

		VertexCount = 0;
		Stride = 0;
		IndexCount = 0;
		SubMeshCount = 0;
	}

public:
	// new
	template<typename T>
	T* NewVertices(UINT count);
	void NewIndices(UINT count);
	void NewSubMesh(UINT count);
	void NewSubMesh();

	// copy
	template<typename T>
	void SetVertices(const T* vertices, UINT count);
	void SetIndices(const UINT* indeces, UINT count);
	void SetSubMesh(const SubMeshData* subMeshes, UINT count);

	// move
	void SetMoveVertices(void** vertices, UINT* stride, UINT* count);
	void SetMoveIndices(UINT** indeces, UINT* count);
	void SetMoveSubMesh(SubMeshData** subMeshes, UINT* count);

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

inline void MeshData::NewSubMesh(UINT count)
{
	SubMeshCount = count;
	SubMeshes = new SubMeshData[count];
}

inline void MeshData::NewSubMesh()
{
	SubMeshCount = 1;
	SubMeshes = new SubMeshData[1];
	SubMeshes->StartIndex = 0u;
	SubMeshes->StartVertex = 0u;
	SubMeshes->IndexCount = IndexCount;
	SubMeshes->VertexCount = VertexCount;
}

// copy
template<typename T>
inline void MeshData::SetVertices(const T * vertices, UINT count)
{
	if (count == 0) return;

	NewVertices<T>(count);
	memcpy(Vertices, vertices, Stride * count);
}

inline void MeshData::SetIndices(const UINT* indeces, UINT count)
{
	if (count == 0) return;

	NewIndices(count);
	memcpy(Indices, indeces, sizeof(UINT) * count);
}

inline void MeshData::SetSubMesh(const SubMeshData * subMeshes, UINT count)
{
	if (count == 0) return;

	NewSubMesh(count);
	memcpy(SubMeshes, subMeshes, sizeof(SubMeshData) * count);
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

inline void MeshData::SetMoveSubMesh(SubMeshData ** subMeshes, UINT * count)
{
	SubMeshes = (*subMeshes);
	SubMeshCount = (*count);
	(*subMeshes) = nullptr;
	(*count) = 0;
}

//static
template<typename T>
inline void MeshData::Copy(MeshData * dst, const MeshData * src)
{
	UINT stride = sizeof(T);
	if (stride != src->Stride) assert(false);  // 잘못 된 카피

	dst->SetVertices<T>((const T *)src->Vertices, src->VertexCount);
	dst->SetIndices(src->Indices, src->IndexCount);
	dst->SetSubMesh(src->SubMeshes, src->SubMeshCount);
}

inline void MeshData::Move(MeshData * dst, MeshData * src)
{
	dst->SetMoveVertices(&src->Vertices, &src->Stride, &src->VertexCount);
	dst->SetMoveIndices(&src->Indices, &src->IndexCount);
	dst->SetMoveSubMesh(&src->SubMeshes, &src->SubMeshCount);
}
