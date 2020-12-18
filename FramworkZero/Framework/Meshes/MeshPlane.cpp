#include "Framework.h"
#include "MeshPlane.h"

typedef Mesh::VertexMesh VertexMesh;

MeshPlane::MeshPlane(float offsetU, float offsetV)
	: offsetU(offsetU), offsetV(offsetV)
{
	// 중점이 0, 0, 0으로 잡음
	// 한 칸 크기 1
	// 기본 10칸

	UINT countX = 11;
	UINT countZ = 11;

	float w = (countX - 1) * 0.5f;
	float h = (countZ - 1) * 0.5f;

	vector<VertexMesh> v;
	for (UINT z = 0; z < countZ; z++)
	{
		for (UINT x = 0; x < countX; x++)
		{
			VertexMesh vertex;
			vertex.Position = Vector3((float)x - w, 0.0f, (float)z - h);
			vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
			vertex.Uv.x = (float)x * offsetU / (float)(countX - 1);
			vertex.Uv.y = (float)z * offsetV / (float)(countZ - 1);
			vertex.Tangent = Vector3(1, 0, 0);

			v.push_back(vertex);
		}
	}

	UINT size;
	size = v.size();
	VertexMesh* vertices = NewVertices<VertexMesh>(size);
	copy
	(
		v.begin(), v.end(), 
		stdext::checked_array_iterator<VertexMesh*>(vertices, size)
	);

	vector<UINT> i;
	for (UINT z = 0; z < countZ - 1; z++)
	{
		for (UINT x = 0; x < countX - 1; x++)
		{
			i.push_back(countX * (z + 0) + x + 0);
			i.push_back(countX * (z + 1) + x + 0);
			i.push_back(countX * (z + 0) + x + 1);

			i.push_back(countX * (z + 0) + x + 1);
			i.push_back(countX * (z + 1) + x + 0);
			i.push_back(countX * (z + 1) + x + 1);
		}
	}

	NewIndices(i.size());
	copy
	(
		i.begin(), i.end(), 
		stdext::checked_array_iterator<UINT*>(Indices, IndexCount)
	);
}

MeshPlane::~MeshPlane()
{
	SafeDeleteData();
}
