#pragma once

class Render2D
{
public:
	Render2D();
	~Render2D();

public:
	void Update();
	void Render();
	void SRV(ID3D11ShaderResourceView* srv);
	Transform* GetTransform() { return transform; }

private:
	ShaderSetter* shader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Transform* transform;
	Matrix world;

	ConstantBuffer* buffer;

private:
	struct VertexTexture
	{
		Vector3 Position;
		Vector2 Uv;
	};

	struct Desc
	{
		Matrix View;
		Matrix Projection;
	}desc;
};