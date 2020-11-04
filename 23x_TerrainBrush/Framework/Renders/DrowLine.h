#pragma once

class DebugLine
{
public:
	friend class Window;

	// - static -
public:
	static DebugLine* Get();

private:
	static void Create();
	static void Delete();

private:
	static DebugLine* instance;

	// - member -
private:
	DebugLine();
	~DebugLine();

public:
	void RenderLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
	void RenderLine(float x1, float y1, float z1, float x2, float y2, float z2);
	void RenderLine(const Vector3& start, const Vector3& end, float r, float g, float b);
	void RenderLine(const Vector3& start, const Vector3& end);
	void RenderLine(const Vector3& start, const  Vector3& end, const Color& color);

private:
	void Render();

private:
	Shader* shader;
	PerFrame* perFrame;
	Transform* transform;

	VertexBuffer* vertexBuffer;

	VertexColor* vertices;
	UINT drawCount = 0;
};