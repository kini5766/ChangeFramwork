#pragma once

class Snow : Renderer
{
public:
	Snow(const Vector3& extent, UINT count, wstring file);
	~Snow();

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
	struct VertexSnow
	{
		Vector3 Position;
		Vector2 Uv;
		float Scale;
		Vector2 Random;
	}* vertices;

	struct Desc
	{
		D3DXCOLOR Color = D3DXCOLOR(0xffffffff);

		Vector3 Velocity = Vector3(0, -5, 0);  // 떨어지는 속도
		float DrawDistance = 0;  // 보여줄 거리

		Vector3 Origin = Vector3(0, 0, 0);  // 떨어지는 중점
		float Turblulence = 5;  // 흔들 거리

		Vector3 Extent = Vector3(0, 0, 0);  // 떨어지는 공간의 크기
		float Padding2;
	}desc;
};