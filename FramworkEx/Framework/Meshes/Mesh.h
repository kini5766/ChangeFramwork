#pragma once

class Mesh : public Renderer
{
public:
	typedef VertexTextureNormal MeshVertex;

public:
	Mesh(Shader* shader);
	virtual ~Mesh();

	void Render();

	void DiffuseMap(wstring file);

protected:
	virtual void Create() = 0;

protected:
	MeshVertex* vertices;
	UINT* indices;

private:
	Texture* diffuseMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};


class Mesh2 : public Renderer
{
public:
	Mesh2(Shader* shader, MeshData* data);
	virtual ~Mesh2();

	void Render();

	void DiffuseMap(wstring file);

private:
	Texture* diffuseMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};
