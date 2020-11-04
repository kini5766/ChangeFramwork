#include "stdafx.h"
#include "IndexDemo.h"

IndexDemo::~IndexDemo()
{
}

void IndexDemo::Initialize()
{
	shader = new Shader(L"04_Index.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

	// ���ؽ� ���� ����
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;  // ���� ũ��
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���뵵

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 2; indices[4] = 1; indices[5] = 3;

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

void IndexDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void IndexDemo::Update()
{
	// ����
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	shader->AsMatrix("World")->SetMatrix(world);

	Color color = Color(0xFF00FF00);
	shader->AsVector("Color")->SetFloatVector(color);
}

void IndexDemo::Render()
{
	UINT stride = sizeof(Vertex);  // �ε��� ¥�� ����
	UINT offset = 0;  // ���� ������

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);  // DXGI : ����Ʈũ��, ǥ������ (gpu�� ����Ÿ��)
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// World
	//shader->Draw(0, 0, 6);  // ��ο��� (1���� ���������� �ϳ� ����)
	shader->DrawIndexed(0, 0, 6);
}

// world ���ϱ� A * B : A�� �ڽ�, B�� �θ�