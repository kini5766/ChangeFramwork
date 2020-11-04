#include "stdafx.h"
#include "GridDemo.h"

GridDemo::~GridDemo()
{
}

void GridDemo::Initialize()
{
	shader = new Shader(L"04_Grid.fx");

	virtexCount = (width + 1) * (height + 1);
	vertices = new Vertex[virtexCount];
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			UINT index = (width + 1) * z + x;  // 12�� ���� ������ ��

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = 0.0f;
			vertices[index].Position.z = (float)z;
		}
	}

	// ���ؽ� ���� ����
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * virtexCount;  // ���� ũ��
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���뵵

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	indexCount = width * height * 6;
	indices = new UINT[indexCount];
	UINT index = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices[index + 0] = (width + 1) * z + x;
			indices[index + 1] = (width + 1) * (z + 1) + x;
			indices[index + 2] = (width + 1) * z + (x + 1);
			indices[index + 3] = (width + 1) * z + (x + 1);
			indices[index + 4] = (width + 1) * (z + 1) + x;
			indices[index + 5] = (width + 1) * (z + 1) + (x + 1);

			index += 6;
		}
	}

	// �ε��� ���� ����
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;  // ���� ũ��
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���뵵

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}
}

void GridDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void GridDemo::Update()
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

void GridDemo::Render()
{
	UINT stride = sizeof(Vertex);  // �ε��� ¥�� ����
	UINT offset = 0;  // ���� ������

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);  // DXGI : ����Ʈũ��, ǥ������ (gpu�� ����Ÿ��)
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// World
	//shader->Draw(0, 0, 6);  // ��ο��� (1���� ���������� �ϳ� ����)
	shader->DrawIndexed(0, 1, indexCount);  // �ε��� ī��Ʈ ������ �ѱ�
}

// world ���ϱ� A * B : A�� �ڽ�, B�� �θ�