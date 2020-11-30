#include "Framework.h"
#include "DrowLine.h"

// 라인 500개가 최대
#define MAX_LINE_VERTEX 1000

// - static -

DebugLine* DebugLine::instance = nullptr;

DebugLine * DebugLine::Get()
{
	assert(instance != nullptr);

	return instance;
}

void DebugLine::Create()
{
	assert(instance == nullptr);

	instance = new DebugLine();
}

void DebugLine::Delete()
{
	SafeDelete(instance);
}

// - member -

DebugLine::DebugLine()
{
	shader = new Shader(L"16_DrowLine.fx");

	vertices = new VertexColor[MAX_LINE_VERTEX];
	ZeroMemory(vertices, sizeof(VertexColor) * MAX_LINE_VERTEX);

	vertexBuffer = new VertexBuffer(vertices, MAX_LINE_VERTEX, sizeof(VertexColor), 0, true);

	perFrame = new PerFrame(shader);
	transform = new Transform(shader);
}

DebugLine::~DebugLine()
{
	SafeDelete(transform);
	SafeDelete(perFrame);
	SafeDelete(vertexBuffer);
	SafeDeleteArray(vertices);
	SafeDelete(shader);
}

void DebugLine::RenderLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	RenderLine(Vector3(x1, y1, z1), Vector3(x2, y2, z2), Color(0, 1, 0, 1));
}

void DebugLine::RenderLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	RenderLine(Vector3(x1, y1, z1), Vector3(x2, y2, z2), Color(r, g, b, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end, float r, float g, float b)
{
	RenderLine(start, end, Color(r, g, b, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end)
{
	RenderLine(start, end, Color(0, 1, 0, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end, const Color & color)
{
	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = start;

	vertices[drawCount].Color = color;
	vertices[drawCount++].Position = end;
}

void DebugLine::Render()
{
	perFrame->Update();
	transform->Update();

	perFrame->Render();
	transform->Render();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(VertexColor) * MAX_LINE_VERTEX);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);

	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(0, 0, drawCount);

	drawCount = 0;
	// ZeroMemory(vertices, sizeof(VertexColor) * MAX_LINE_VERTEX);
}
