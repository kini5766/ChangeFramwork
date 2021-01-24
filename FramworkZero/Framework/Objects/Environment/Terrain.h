#pragma once

constexpr auto TERRAIN_TEXTURE_HEIGHT = 32.0f;

class Terrain
{
public:
	struct VertexTerrain;
		
public:
	Terrain(wstring imageFile);
	~Terrain();

public:
	void Update();
	void PreRender_Depth();
	void Render();
	void RenderVisibleNormal();

	void RecalculateNormals();
	void ApplyVertex();


	// Getter, Setter
public:
	void BaseMap(wstring file);
	UINT Width() const { return width; }
	UINT Height() const { return height; }
	VertexTerrain* Vertices() { return vertices; }
	const MeshData* GetMeshData() const { return meshData; }

	Shader* GetShader() { return shader; }
	void Pass(UINT value) { pass = value; }
	Transform* GetTransform() { return perTransform->GetTransform(); }

	void GetHeights(UINT ** heights);

private:
	void ReadHeightData();
	void CreateVertexData();
	void CreateIndexData();


private:
	VertexTerrain* vertices;
	Texture* baseMap = nullptr;
	float* heights;  // 하이트맵의 높이
	UINT width, height;  // 하이트맵 기준으로 생성 (한 칸 모자라게)
	wstring imageFile;

	// render
private:
	Shader* shader;
	Material* material;
	MeshData* meshData;
	IndexBuffer* indexBuffer;
	VertexBuffer* vertexBuffer;
	PerTransform* perTransform;
	UINT pass = 3;
	ShadowTest* shadow;

public:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
		Color _Color = Color(0, 0, 0, 0);
	};

	//operator TerrainCollider();
};

#include "TerrainCollider.h"