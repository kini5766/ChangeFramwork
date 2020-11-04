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

		Vector3 Velocity = Vector3(0, -5, 0);  // �������� �ӵ�
		float DrawDistance = 0;  // ������ �Ÿ�

		Vector3 Origin = Vector3(0, 0, 0);  // �������� ����
		float Turblulence = 5;  // ��� �Ÿ�

		Vector3 Extent = Vector3(0, 0, 0);  // �������� ������ ũ��
		float Padding2;
	}desc;
};