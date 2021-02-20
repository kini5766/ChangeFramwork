#pragma once

class MeshInstancing
{
public:
	MeshInstancing(unique_ptr<MeshData> shape);
	~MeshInstancing();

public:
	void Update();
	void UpdateTransforms();

	void Render();
	void Render_CubeMap();
	void Render_EnvCube();

public:
	MeshInstance* AddInstance();
	MeshInstRenderer* GetRenderer() const { return renderer; }
	Material* GetMaterial() { return material; }


private:
	Shader* shader;
	Material* material;
	PerFrame* perframe;
	MeshInstRenderer* renderer;

private:
	void PreRender_Depth();
	void PreRender_EnvCube();
	ShadowCaster* shadowCaster;
	EnvCubeCaster* envCubeCaster;
};