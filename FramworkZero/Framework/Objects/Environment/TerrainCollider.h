#pragma once

class TerrainCollider
{
public:
	Terrain::VertexTerrain* Vertices;
	Transform* Transform;
	UINT Width;
	UINT Height;
	const MeshData* MeshData;

	// Picker
public:
	float GetHeight(const Vector3& position);  // 지면 1칸의 크기가 1일때만 사용가능
	Vector3 GetNormal(float x, float z);
	float GetVerticalRaycast(const Vector3& position);
	bool GetRaycastPosition_Old(Vector3* outPoint, bool bTest = false);
	bool GetMouseRaycast(Vector3* outPoint, bool bTest = false);
	bool GetMouseRaycast2(Vector3* outPoint);

public:
	TerrainCollider() {}
	TerrainCollider(Terrain& value);
};