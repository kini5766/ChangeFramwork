#pragma once

struct BrushDesc
{
	Color Color = D3DXCOLOR(0, 0.75f, 0, 1);

	Vector3 Location;
	UINT Shape = 0;
	UINT Range = 2;

	float Rad = 0.5f;
	float DragLength = 0.0f;

	float Padding[1];
};


class Brush
{
public:
	Brush();
	~Brush();

public:
	void SetTerrain(TerrainLOD* value);
	TerrainLOD* GetTerrain() { return terrain; }

public:
	void Update();
	void Render();
	void RenderImGui();

private:
	void UpdateBrush();
	void UpdatePickMode();

private:
	void ApplyBrush();

private:
	TerrainLOD* terrain;
	ShaderSetter* shader;
	ConstantBuffer* brushBuffer;
	ConstantBuffer* lineBuffer;

private:
	BrushDesc brushDesc;
	struct BrushEditor* brush;
	class BrushInput* input;
	class TerrainPicker* picker;

private:
	function<bool(Vector3*)> funcRaycast;

private:
	struct LineDesc
	{
		Color Color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		UINT Visible = 1;
		float Thickness = 0.99f;
		float Size = 15.0f;
		float Padding;
	}lineDesc;

private:
	int targetLayer = -1;
};