#include "stdafx.h"
#include "RectDemo.h"

RectDemo::~RectDemo()
{
}

void RectDemo::Initialize()
{
	shader = new Shader(L"02_Vertex.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Color = Color(0x00FFFFFF);
	vertices[1].Color = Color(0xFF00FFFF);
	vertices[2].Color = Color(0xFFFF00FF);
	vertices[3].Color = Color(0xFFFF00FF);
	vertices[4].Color = Color(0xFF00FFFF);
	vertices[5].Color = Color(0x00FFFFFF);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(VertexColor) * 6;  // 보낼 크기
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 사용용도
	
	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer))
}

void RectDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void RectDemo::Update()
{
}

void RectDemo::Render()
{
	UINT stride = sizeof(VertexColor);  // 인덱스 짜를 기준
	UINT offset = 0;  // 읽을 시작점

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static Color color = Color(1, 1, 1, 1);
	ImGui::ColorEdit3("Color", color);
	shader->AsVector("Color")->SetFloatVector(color);
	
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->Draw(0, (bWireFrame)? 1 : 0, 6);  // 드로우콜 (1번이 파이프라인 하나 실행)
}
