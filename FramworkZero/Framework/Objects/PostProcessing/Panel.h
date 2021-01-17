#pragma once

class Panel
{
public:
	Panel(Shader* shader);
	~Panel();

public:
	void Update();
	void Render();

public:
	void Pass(UINT value) { pass = value; }
	UINT Pass() { return pass; }
	void SRV(ID3D11ShaderResourceView* srv);

private:
	UINT pass = 0;
	ShaderSetter* material;
	VertexBuffer* vertexBuffer;
	PerFrame* perFrame;
	
private:
	struct Vertex
	{
		Vector3 Position;
	};
};
