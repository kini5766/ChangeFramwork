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
			UINT index = (width + 1) * z + x;  // 12번 줄의 저차원 값

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = 0.0f;
			vertices[index].Position.z = (float)z;
		}
	}

	// 버텍스 버퍼 생성
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * virtexCount;  // 보낼 크기
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 사용용도

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

	// 인덱스 버퍼 생성
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;  // 보낼 크기
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 사용용도

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

	// 관점
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
	UINT stride = sizeof(Vertex);  // 인덱스 짜를 기준
	UINT offset = 0;  // 읽을 시작점

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);  // DXGI : 바이트크기, 표현범위 (gpu의 변수타입)
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// World
	//shader->Draw(0, 0, 6);  // 드로우콜 (1번이 파이프라인 하나 실행)
	shader->DrawIndexed(0, 1, indexCount);  // 인덱스 카운트 개수를 넘김
}

// world 곱하기 A * B : A가 자식, B가 부모