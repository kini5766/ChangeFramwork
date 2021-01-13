#pragma once

constexpr auto MAX_DEBUG_LINE = 150000;

class DebugLine
{
public:
	friend class Window;  // 윈도우만 싱글톤 생성

private:
	struct VertexColor;

private:
	DebugLine();
	~DebugLine();

public:
	void RenderLine(float x, float y, float z, float x2, float y2, float z2);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, const Color& color);
	void RenderLine(float x, float y, float z, float x2, float y2, float z2, float r, float g, float b);

	void RenderLine(const Vector3& start, const Vector3& end);
	void RenderLine(const Vector3& start, const Vector3& end, float r, float g, float b);
	void RenderLine(const Vector3& start, const Vector3& end, const Color& color);

private:  // Window
	void Update();
	void Render();


private:
	const UINT stride;  // sizeof(VertexColor)

private:
	ShaderSetter* shader;
	VertexColor* vertices;
	UINT drawCount = 0;  // 랜더 후 0으로 초기화됨

	VertexBuffer* vertexBuffer;
	PerFrame* perFrame;

private:
	struct VertexColor
	{
		VertexColor()
			: Position(0, 0, 0), Color(0, 0, 0, 1) {}

		VertexColor(float x, float y, float z, float r, float g, float b)
		{
			Position.x = x;
			Position.y = y;
			Position.z = z;

			Color.r = r;
			Color.g = g;
			Color.b = b;
			Color.a = 1.0f;
		}

		Vector3 Position;
		Color Color;
	};
};