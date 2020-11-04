#include "stdafx.h"
#include "VertexDemo.h"

VertexDemo::~VertexDemo()
{
}

void VertexDemo::Initialize()
{
	shader = new Shader(L"02_Vertex.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3( 0.0f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 3;  // 보낼 크기
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 사용용도
	
	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer))
}

void VertexDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void VertexDemo::Update()
{
}

void VertexDemo::Render()
{
	UINT stride = sizeof(Vertex);  // 인덱스 짜를 기준
	UINT offset = 0;  // 읽을 시작점

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->Draw(0, (bWireFrame)? 1 : 0, 3);  // 드로우콜 (1번이 파이프라인 하나 실행)
}
