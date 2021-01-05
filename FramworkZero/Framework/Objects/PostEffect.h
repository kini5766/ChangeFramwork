#pragma once

class PostEffect
{
public:
	PostEffect(wstring shaderFile);
	~PostEffect();

public:
	void Update();
	void Render();

	void SRV(ID3D11ShaderResourceView* srv);

private:
	UINT pass = 0;
	ShaderSetter* shader;
	VertexBuffer* vertexBuffer;

private:
	struct Vertex
	{
		Vector3 Position;
	};
};
