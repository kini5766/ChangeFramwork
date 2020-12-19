#include "Framework.h"
#include "MeshCube.h"

typedef Mesh::VertexMesh VertexMesh;

MeshCube::MeshCube()
{
	vector<VertexMesh> v;

	float w = 0.5f, h = 0.5f, d = 0.5f;

	//Front
	v.push_back(VertexMesh(Vector3(-w, -h, -d), Vector2(0, 1), Vector3(0, 0, -1), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(-w, +h, -d), Vector2(0, 0), Vector3(0, 0, -1), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, +h, -d), Vector2(1, 0), Vector3(0, 0, -1), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, -h, -d), Vector2(1, 1), Vector3(0, 0, -1), Vector3(1, 0, 0)));

	//Back
	v.push_back(VertexMesh(Vector3(-w, -h, +d), Vector2(1, 1), Vector3(0, 0, 1), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, -h, +d), Vector2(0, 1), Vector3(0, 0, 1), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, +h, +d), Vector2(0, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(-w, +h, +d), Vector2(1, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0)));

	//Top
	v.push_back(VertexMesh(Vector3(-w, +h, -d), Vector2(0, 1), Vector3(0, 1, 0), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(-w, +h, +d), Vector2(0, 0), Vector3(0, 1, 0), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, +h, +d), Vector2(1, 0), Vector3(0, 1, 0), Vector3(1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, +h, -d), Vector2(1, 1), Vector3(0, 1, 0), Vector3(1, 0, 0)));

	//Bottom
	v.push_back(VertexMesh(Vector3(-w, -h, -d), Vector2(1, 1), Vector3(0, -1, 0), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, -h, -d), Vector2(0, 1), Vector3(0, -1, 0), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(+w, -h, +d), Vector2(0, 0), Vector3(0, -1, 0), Vector3(-1, 0, 0)));
	v.push_back(VertexMesh(Vector3(-w, -h, +d), Vector2(1, 0), Vector3(0, -1, 0), Vector3(-1, 0, 0)));

	//Left
	v.push_back(VertexMesh(Vector3(-w, -h, +d), Vector2(0, 1), Vector3(-1, 0, 0), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(-w, +h, +d), Vector2(0, 0), Vector3(-1, 0, 0), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(-w, +h, -d), Vector2(1, 0), Vector3(-1, 0, 0), Vector3(0, 0, -1)));
	v.push_back(VertexMesh(Vector3(-w, -h, -d), Vector2(1, 1), Vector3(-1, 0, 0), Vector3(0, 0, -1)));

	//Right
	v.push_back(VertexMesh(Vector3(+w, -h, -d), Vector2(0, 1), Vector3(1, 0, 0), Vector3(0, 0, 1)));
	v.push_back(VertexMesh(Vector3(+w, +h, -d), Vector2(0, 0), Vector3(1, 0, 0), Vector3(0, 0, 1)));
	v.push_back(VertexMesh(Vector3(+w, +h, +d), Vector2(1, 0), Vector3(1, 0, 0), Vector3(0, 0, 1)));
	v.push_back(VertexMesh(Vector3(+w, -h, +d), Vector2(1, 1), Vector3(1, 0, 0), Vector3(0, 0, 1)));

	UINT size = v.size();
	VertexMesh* vertices = NewVertices<VertexMesh>(size);
	copy(v.begin(), v.end(), stdext::checked_array_iterator<VertexMesh*>(vertices, size));

	// Front 0 1 2 2 1 3
	// Back 4 5 6 6 5 7
	// Top 8 9 10 10 9 11
	// Bottom 12 13 14 14 13 15
	// Left 16 17 18 18 17 19
	// Right 20 21 22 22 21 23

	IndexCount = 36;
	Indices = new UINT[IndexCount]
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};
}

MeshCube::~MeshCube()
{
	SafeDeleteData();
}
