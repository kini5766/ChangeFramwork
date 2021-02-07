#include "Framework.h"
#include "Render2D.h"

using namespace ShaderEffectName;

Render2D::Render2D()
{
	shader = Shader::Load(L"01_Render2D.fxo");
	mesh = new Mesh(shader, CreateMeshData());
}

Render2D::~Render2D()
{
	SafeDelete(mesh);
	SafeRelease(shader);
}

void Render2D::Update()
{
	mesh->Update();
}

void Render2D::PostRender()
{
	mesh->Render();
}

void Render2D::SRV(ID3D11ShaderResourceView * srv)
{
	mesh->GetMaterial()->SetSRV(DIFFUSEMAP, srv);
}

MeshData Render2D::CreateMeshData()
{
	MeshData result;

	VertexTexture* vertices = result.NewVertices<VertexTexture>(4);
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 0);

	UINT indices[6] = { 0 ,2, 1, 2, 3, 1 };
	result.SetIndices(indices, 6);

	return result;
}
