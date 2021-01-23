#pragma once

struct BrushEditor
{
public:
	void Set();
	void RenderImGui();

public:
	struct BrushDesc& Desc;
	float Intensity = 5.0f;
	UINT Type = 0;

	// terrain
	UINT Width;
	UINT Height;
	const Terrain::VertexTerrain* Origin;
	Terrain::VertexTerrain* Result;

	// flat
	float FlatHeight = 0.0f;

	// drag
	Vector3 DragStartPoint;

	// drag result
	Plane PlaneSlope;
	Vector2 Vec2Rad;

private:
	// 시작
	void RaiseShape();
	// 모양 -> 변형
	void RaiseType(UINT x, UINT z, float intensity);

	// 모양
private:
	void RaiseRect();
	void RaiseCircle();
	void RaiseDrag();

private:
	// 변형
	void RaiseUp(UINT x, UINT z, float intensity);
	void RaiseDown(UINT x, UINT z, float intensity);
	void RaiseNoise(UINT x, UINT z, float intensity);
	void RaiseSmoothing(UINT x, UINT z, float intensity);
	void RaiseFlat(UINT x, UINT z, float intensity);
	void RaiseSlope(UINT x, UINT z, float intensity);
};