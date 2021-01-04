#include "Framework.h"
#include "Render2D.h"

#include "Tools/Viewer/CanvasCamera.h"

Render2D::Render2D()
{
	shader = new ShaderSetter(L"01_Render2D.fxo");
	transform = new Transform(&world);
	shader->SetMatrixPointer("World", &world);

	D3DXMatrixLookAtLH
	(
		&desc.View,
		&Vector3(0, 0, 0),
		&Vector3(0, 0, 1),
		&Vector3(0, 1, 0)
	);
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	shader->SetConstantBuffer("CB_Render2D", buffer->Buffer());

	VertexTexture vertices[4];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 0);

	vertexBuffer = new VertexBuffer(vertices, 4, sizeof(VertexTexture));

	UINT indices[6] = { 0 ,2, 1, 2, 3, 1 };

	indexBuffer = new IndexBuffer(indices, 6);

	shader->SetSRV("DiffuseMap", nullptr);
}

Render2D::~Render2D()
{
	SafeDelete(shader);
	SafeDelete(buffer);
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);
}

void Render2D::Update()
{
	desc.View = Context::Get()->Canvas()->View();
	desc.Projection = Context::Get()->Canvas()->Projection();
	transform->UpdateWorld();
}

void Render2D::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->Render();

	buffer->Render();
	shader->GetShader()->DrawIndexed(0, 0, 6);
}

void Render2D::SRV(ID3D11ShaderResourceView * srv)
{
	shader->SetSRV("DiffuseMap", srv);
}
