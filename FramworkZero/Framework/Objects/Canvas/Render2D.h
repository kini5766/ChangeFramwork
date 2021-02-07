#pragma once

class Render2D
{
public:
	Render2D();
	~Render2D();

public:
	void Update();
	void PostRender();
	void SRV(ID3D11ShaderResourceView* srv);
	Transform* GetTransform() { return mesh->GetTransform(); }

private:
	MeshData CreateMeshData();

private:
	Shader* shader;
	Mesh* mesh;

private:
	struct VertexTexture
	{
		Vector3 Position;
		Vector2 Uv;
	};
};