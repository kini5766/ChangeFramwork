#include "Framework.h"
#include "MeshQuad.h"

MeshQuad::MeshQuad()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<VertexMesh> v;
	v.push_back(VertexMesh(-w, -h, 0, 0, 1, 0, 0, -1, 1, 0, 0));
	v.push_back(VertexMesh(-w, +h, 0, 0, 0, 0, 0, -1, 1, 0, 0));
	v.push_back(VertexMesh(+w, -h, 0, 1, 1, 0, 0, -1, 1, 0, 0));
	v.push_back(VertexMesh(+w, +h, 0, 1, 0, 0, 0, -1, 1, 0, 0));

	UINT size = v.size();
	VertexMesh* vertices = NewVertices<VertexMesh>(size);
	copy(v.begin(), v.end(), stdext::checked_array_iterator<VertexMesh*>(vertices, size));

	IndexCount = 6;
	Indices = new UINT[IndexCount]{0,1,2,2,1,3};
}

MeshQuad::~MeshQuad()
{
	SafeDeleteData();
}
