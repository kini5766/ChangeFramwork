#include "stdafx.h"
#include "TextureDemo.h"

#include "Viewer/Freedom.h"

TextureDemo::~TextureDemo()
{
}

void TextureDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 0, -30);

	shader = new Shader(L"08_Texture.fx");

	texture = new Texture(L"Stones.png");
	sTexture = shader->AsSRV("Texture");

	VertexTexture vertices[4];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

	// Address
	//vertices[0].Uv = Vector2(0.0f, 2.0f);
	//vertices[1].Uv = Vector2(0.0f, 0.0f);
	//vertices[2].Uv = Vector2(2.0f, 2.0f);
	//vertices[3].Uv = Vector2(2.0f, 0.0f);

	// Filter
	vertices[0].Uv = Vector2(0.0f, 1.0f);
	vertices[1].Uv = Vector2(0.0f, 0.0f);
	vertices[2].Uv = Vector2(1.0f, 1.0f);
	vertices[3].Uv = Vector2(1.0f, 0.0f);

	UINT indices[6] =
	{ 0, 1, 2, 2, 1, 3 };

	// ���ؽ� ���� ����
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VertexTexture) * 4;  // ���� ũ��
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���뵵

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	// �ε��� ���� ����
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;  // ���� ũ��
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���뵵

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}
}

void TextureDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(texture);
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void TextureDemo::Update()
{
	Matrix world;

	D3DXMatrixScaling(&world, 10, 10, 1);
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void TextureDemo::Render()
{
	UINT stride = sizeof(VertexTexture);  // �ε��� ¥�� ����
	UINT offset = 0;  // ���� ������

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);  // DXGI : ����Ʈũ��, ǥ������ (gpu�� ����Ÿ��)
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	sTexture->SetResource(texture->SRV());
	
	//static UINT address = 0;
	//ImGui::InputInt("Filter", (int*)&address);
	//address %= 4;
	//
	//shader->AsScalar("Address")->SetInt(address);
	//shader->DrawIndexed(0, 1, 6);

	static UINT filter = 0;
	ImGui::InputInt("Filter", (int*)&filter);
	filter %= 3;

	shader->AsScalar("Filter")->SetInt(filter);
	shader->DrawIndexed(0, 2, 6);
}

// world ���ϱ� A * B : A�� �ڽ�, B�� �θ�