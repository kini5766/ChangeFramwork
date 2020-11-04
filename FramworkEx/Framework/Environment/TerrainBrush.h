#pragma once

typedef VertexTextureNormal TerrainVertex;

class TerrainBrushDemo
{
public:
	TerrainBrushDemo(Shader * shader);
	~TerrainBrushDemo();
	
public:
	void SetVertex(const TerrainVertex* vertices, UINT width, UINT height);
	void StartBrush(const Vector3& position);
	void UpdateBrush(TerrainVertex* outVertices);
	void EndBrush();
	void Render();
	void RenderImGui();

private:
	struct BrushDesc;
public:
	void SetColor(const Color& value) { brushDesc.Color = value; }
	void SetLocation(const Vector3& value) { brushDesc.Location = value; }
	void Type(UINT value) { type = value; if (value == 5)brushDesc.Shape = 3; }
	UINT Type() { return type; }

	void Shape(UINT value) { brushDesc.Shape = value; if (type == 5)brushDesc.Shape = 3; }
	UINT Shape() { return brushDesc.Shape; }
	void Range(UINT value) { brushDesc.Range = value; }
	UINT Range() { return brushDesc.Range; }
	void Rad(float value) { brushDesc.Rad = value; }
	float Rad() { return brushDesc.Rad; }

private:
	void RaiseRect(const Vector3 & position, UINT range);
	void RaiseCircle(const Vector3 & position, UINT range);
	void RaiseDrag(const Vector3 & position, UINT range);

	void RaiseType(UINT x, UINT z);

	void RaiseUp(float* inout);
	void RaiseDown(float* inout);
	void RaiseNoise(float* inout);
	void RaiseSmoothing(float * inout, UINT x, UINT z);
	void RaiseFlat(float * inout);
	void RaiseSlope(float * inout, UINT x, UINT z);

private:
	const TerrainVertex* original = nullptr;
	TerrainVertex* terrainVertex = nullptr;
	UINT width = 0;
	UINT height = 0;
	float flatHeight;
	UINT type = 0;

	// slope
	Vector3 startDragPoint;
	Plane* planeSlope = nullptr;

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0xFF00FF00);
		Vector3 Location;
		UINT Shape = 0;
		UINT Range = 1;
		float Rad = 0.5f;
		float DragLength = 0.0f;
		float Padding[1];
	}brushDesc;
	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;
};