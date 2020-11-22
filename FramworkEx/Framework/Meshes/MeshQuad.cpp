#include "Framework.h"
#include "MeshQuad.h"

#include "Mesh.h"
typedef Mesh::MeshVertex MeshVertex;

MeshQuad::MeshQuad()
{
	Create();
}

MeshQuad::~MeshQuad()
{
}

void MeshQuad::Create()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<MeshVertex> v;
	v.push_back(MeshVertex(-w, -h, 0, 0, 1, 0, 0, -1));
	v.push_back(MeshVertex(-w, +h, 0, 0, 0, 0, 0, -1));
	v.push_back(MeshVertex(+w, -h, 0, 1, 1, 0, 0, -1));
	v.push_back(MeshVertex(+w, +h, 0, 1, 0, 0, 0, -1));

	//NewCopyVertices<MeshVertex>(v, v.size());
	VertexCount = v.size();
	Vertices = new MeshVertex[VertexCount];
	memcpy(Vertices, v.data(), sizeof(MeshVertex) * VertexCount);

	IndexCount = 6;
	Indices = new UINT[IndexCount]
	{
		0, 1, 2, 2, 1, 3
	};
}
