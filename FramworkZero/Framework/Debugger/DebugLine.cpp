#include "Framework.h"
#include "DebugLine.h"

DebugLine::DebugLine()
	: stride(sizeof(VertexColor))
{
	shader = new ShaderSetter(L"01_DebugLine.fxo");

	// create vertex
	vertices = new VertexColor[MAX_DEBUG_LINE];
	ZeroMemory(vertices, sizeof(VertexColor) * MAX_DEBUG_LINE);
	vertexBuffer = new VertexBuffer(vertices, MAX_DEBUG_LINE, sizeof(VertexColor), 0, true);

	perFrame = new PerFrame(shader->GetShader());
}

DebugLine::~DebugLine()
{
	SafeDelete(vertexBuffer);
	SafeDeleteArray(vertices);
	SafeDelete(shader);
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2));
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, const Color & color)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2), color);
}

void DebugLine::RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b)
{
	RenderLine(Vector3(x, y, z), Vector3(x2, y2, z2), Color(r, g, b, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end)
{
	RenderLine(start, end, Color(0, 1, 0, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end, float r, float g, float b)
{
	RenderLine(start, end, Color(r, g, b, 1));
}

void DebugLine::RenderLine(const Vector3 & start, const Vector3 & end, const Color & color)
{
	if (bDrawing == false)
		return;

	vertices[drawCount].Position = start;
	vertices[drawCount++].Color = color;  // 맨 밑에 증가 연산자 drowCount++

	vertices[drawCount].Position = end;
	vertices[drawCount++].Color = color;  // 맨 밑에 증가 연산자 drowCount++
}

void DebugLine::Update()
{
	perFrame->Update();
}

void DebugLine::Render()
{
	if (drawCount < 1) return;

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(VertexColor) * MAX_DEBUG_LINE);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);

	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	perFrame->Render();
	shader->GetShader()->Draw(0, 0, drawCount);  // 반복문 드로우 콜 보단 한 번에 보내는 것이 좋음

	drawCount = 0;  // 처음 부터 그림
}
