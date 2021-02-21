#pragma once

class Water
{
public:
	struct InitializeDesc;

public:
	struct InitializeDesc
	{
		float Radius;
		float Width = 0;
		float Height = 0;
	};

public:
	Water(const InitializeDesc& initDesc);
	~Water();

public:
	void Update();
	void Render();

	void PreRender_Reflection();
	void PreRender_Refraction();

	Material* GetMaterial() { return mesh->GetMaterial(); }
	Transform* GetTransform() { return mesh->GetTransform(); }

private:
	MeshData MakeMeshData(float radius);
	//void PreRender_EnvCube();

private:
	Shader* shader;
	Mesh* mesh;
	ConstantBuffer* buffer;
	class Reflection* reflection;
	class Refraction* refraction;

	Texture* waterMap;
	float waveSpeed = 0.06f;
	float radius;

	//EnvCubeCaster* preEnvCube;

private:
	struct Desc
	{
		Vector2 NormalMapTile = Vector2(4, 8);
		float WaveTransform = 0.0f;
		float WaveScale = 0.05f;
	}desc;

private:
	struct VertexTexture
	{
		Vector3 Position;
		Vector2 Uv;
	};

};