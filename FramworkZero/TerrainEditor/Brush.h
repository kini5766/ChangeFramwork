#pragma once

class Brush
{
public:
	Brush(Terrain* terrain);
	~Brush();

public:
	void Update();
	void Render();

private:
	void RaiseHeight(float intensity);

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		
		Vector3 Location;
		UINT Type = 0;

		UINT Range = 2;

		float Padding[3];
	}brushDesc;

private:
	ShaderSetter* shader;
	Terrain* terrain;
	ConstantBuffer* brushBuffer;
};