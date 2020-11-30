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

		Vector3 Velocity = Vector3(-10, -100, 0);  // �������� �ӵ�
		float DrawDistance = 0;  // ������ �Ÿ�

		Vector3 Origin = Vector3(0, 0, 0);  // �������� ����
		float Padding;

		Vector3 Extent = Vector3(0, 0, 0);  // �������� ������ ũ��
		float Padding2;
	}desc;
};