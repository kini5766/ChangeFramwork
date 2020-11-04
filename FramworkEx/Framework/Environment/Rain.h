#pragma once

class Rain : Renderer
{
public:
	Rain(const Vector3& extent, UINT count, wstring file);
	~Rain();

public:
	void Update();
	void Render();

private:
	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	UINT* indices;

	Texture* texture;
	UINT drawCount;

private:
	struct VertexRain
	{
		Vector3 Position;
		Vector2 Uv;
		Vector2 Scale;
	}* vertices;

	struct Desc
	{
		D3DXCOLOR Color = D3DXCOLOR(0xffffffff);

		Vector3 Velocity = Vector3(-10, -100, 0);  // 떨어지는 속도
		float DrawDistance = 0;  // 보여줄 거리

		Vector3 Origin = Vector3(0, 0, 0);  // 떨어지는 중점
		float Padding;

		Vector3 Extent = Vector3(0, 0, 0);  // 떨어지는 공간의 크기
		float Padding2;
	}desc;
};