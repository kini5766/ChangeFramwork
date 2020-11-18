#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Shader * shader)
	: Renderer(shader)
{
	sDiffuseMap = shader->AsSRV("DiffuseMap");
}

Mesh::~Mesh()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(diffuseMap);
}

void Mesh::Render()
{
	if (vertexBuffer == nullptr || indexBuffer == nullptr)
	{
		Create();  // 순수 가상함수는 생성자에 호출 안됨
		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(MeshVertex));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	Renderer::Update();
	Renderer::Render();

	if (diffuseMap != nullptr)
		sDiffuseMap->SetResource(diffuseMap->SRV());
	shader->DrawIndexed(0, Pass(), indexCount);
}

void Mesh::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	diffuseMap = new Texture(file);
}


// Mesh2

Mesh2::Mesh2(Shader * shader, MeshData* data)
	: Renderer(shader)
{
	sDiffuseMap = shader->AsSRV("DiffuseMap");
	vertexCount = data->VertexCount;
	indexCount = data->IndexCount;
	vertexBuffer = new VertexBuffer(data->Vertices, vertexCount, data->Stride);
	indexBuffer = new IndexBuffer(data->Indices, indexCount);
}

Mesh2::~Mesh2()
{
	SafeDelete(diffuseMap);
}

void Mesh2::Render()
{
	Renderer::Update();
	Renderer::Render();

	if (diffuseMap != nullptr)
		sDiffuseMap->SetResource(diffuseMap->SRV());
	shader->DrawIndexed(0, Pass(), indexCount);
}

void Mesh2::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	diffuseMap = new Texture(file);
}
