#pragma once

class Mesh
{
public:
	Mesh(Shader* shader, MeshData&& meshMove);
	~Mesh();

public:
	void Update();
	void Render();

public:
	Material* GetMaterial() { return material; }
	Transform* GetTransform() { return transform; }
	void Pass(UINT value) { return renderer->Pass(value); }

private:
	MeshData data;

	MeshRenderer* renderer;
	PerTransform* perTransform;

	Material* material;
	Transform* transform;
};