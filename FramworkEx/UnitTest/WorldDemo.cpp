#include "stdafx.h"
#include "WorldDemo.h"

WorldDemo::~WorldDemo()
{
}

void WorldDemo::Initialize()
{
	shader = new Shader(L"03_World.fx");

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

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&world2);
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void WorldDemo::Update()
{
	// 관점
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
	shader->AsMatrix("World")->SetMatrix(world);
}

void WorldDemo::Render()
{
	UINT stride = sizeof(VertexColor);  // 인덱스 짜를 기준
	UINT offset = 0;  // 읽을 시작점

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// World
	{
		Color color = Color(0, 1, 0, 1);

		static Vector3 position = Vector3(0, 0, 0);
		ImGui::SliderFloat3("Position", position, -10, 10);

		world._41 = position.x;
		world._42 = position.y;
		world._43 = position.z;

		shader->AsVector("Color")->SetFloatVector(color);
		shader->AsMatrix("World")->SetMatrix(world);
	}
	shader->Draw(0, 0, 6);  // 드로우콜 (1번이 파이프라인 하나 실행)

	// World
	{
		Color color = Color(0, 0, 1, 1);

		static Vector3 position2 = Vector3(3, 3, 0);
		ImGui::SliderFloat3("Position2", position2, -10, 10);

		world2._41 = position2.x;
		world2._42 = position2.y;
		world2._43 = position2.z;

		shader->AsVector("Color")->SetFloatVector(color);
		shader->AsMatrix("World")->SetMatrix(world2);
	}

	shader->Draw(0, 0, 6);  // 드로우콜 (1번이 파이프라인 하나 실행)
}

// world 곱하기 A * B : A가 자식, B가 부모