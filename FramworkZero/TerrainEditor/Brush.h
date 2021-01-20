#pragma once

class Brush
{
public:
	Brush();
	~Brush();

public:
	void Update();
	void Render();
	void RenderImGui();

public:
	void SetTerrain(Terrain* value);

private:
	void UpdateBrush();

	// 모양
private:
	void RaiseRect(const Vector3 & position, UINT range) {}
	void RaiseCircle(const Vector3 & position, UINT range) {}
	void RaiseDrag(const Vector3 & position, UINT range) {}

private:
	// 중간 다리
	void RaiseType(UINT x, UINT z) {}

private:
	// 방법
	void RaiseUp(UINT x, UINT z, float intensity);
	void RaiseDown(UINT x, UINT z, float intensity);
	void RaiseNoise(UINT x, UINT z, float intensity);
	void RaiseSmoothing(UINT x, UINT z, float intensity);
	void RaiseFlat(UINT x, UINT z, float intensity);
	void RaiseSlope(UINT x, UINT z, float intensity);

private:
	ShaderSetter* shader;
	Terrain* terrain;
	ConstantBuffer* brushBuffer;
	ConstantBuffer* lineBuffer;

private:
	const Terrain::VertexTerrain* origin;
	Terrain::VertexTerrain* result;
	Vector3 prevMousePos;
	UINT type = 0;

	// flat
	float flatHeight;

	// slope
	Vector3 startDragPoint;
	Plane* planeSlope = nullptr;

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);

		Vector3 Location;
		UINT Shape = 0;
		UINT Range = 2;

		float Rad = 0.5f;
		float DragLength = 0.0f;

		float Padding[1];
	}brushDesc;
	struct LineDesc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		UINT Visible = 1;
		float Thickness = 0.01f;
		float Size = 1.0f;
		float Padding;
	}lineDesc;
};