#pragma once

class Renderer
{
public:
	Renderer(Shader* shader);
	Renderer(wstring shaderFile);
	virtual ~Renderer();

	virtual void Update();
	virtual void Render();


	UINT& Pass() { return pass; }
	void Pass(UINT value) { pass = value; }
	Shader* GetShader() { return shader; }
	Transform* GetTransform() { return transform; }

private:
	void Initialize();

protected:
	Shader* shader;

	Transform* transform;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	UINT vertexCount = 0;
	UINT indexCount = 0;

private:
	bool bCreateShader = false;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT pass = 0;

	PerFrame* perFrame;
};