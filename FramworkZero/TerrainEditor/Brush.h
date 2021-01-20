#pragma once

class Brush
{
public:
	Brush();
	~Brush();

public:
	void Update();
	void Render();

public:
	void SetTerrain(Terrain* value);

private:
	void RaiseHeight(float intensity);

private:
	ShaderSetter* shader;
	Terrain* terrain;
	ConstantBuffer* brushBuffer;
	ConstantBuffer* lineBuffer;

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);

		Vector3 Location;
		UINT Type = 0;

		UINT Range = 2;

		float Padding[3];
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