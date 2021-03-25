#pragma once

class TerrainLodCollider 
{ 
public:
	bool GetRaycastPosition_Old(Vector3* outPoint);

public:
	float* Heights;
	float HeightScale;
	Transform* Transform;

	UINT MapWidth;
	UINT MapHeight;
	float Width;
	float Height;

public:
	TerrainLodCollider();
	TerrainLodCollider(TerrainLOD& value);
};


class TerrainCollider
{
public:
	Terrain::VertexTerrain* Vertices = nullptr;
	Transform* Transform = nullptr;
	UINT Width = 0;
	UINT Height = 0;
	const MeshData* MeshData = nullptr;

	// Picker
public:
	float GetHeight(const Vector3& position);  // 지면 1칸의 크기가 1일때만 사용가능
	Vector3 GetNormal(float x, float z);
	float GetVerticalRaycast(const Vector3& position);
	bool GetMouseRaycast(Vector3* outPoint);
	bool GetMouseRaycast2(Vector3* outPoint);

public:
	TerrainCollider() {}
	TerrainCollider(Terrain& value);
};
