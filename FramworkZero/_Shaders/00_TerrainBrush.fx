
// --
// Brush
// --

struct BrushDesc
{
	float4 Color;
	float3 Location;
	uint Type;
	uint Range;
	//float Padding[3];
};

cbuffer CB_TerrainBrush
{
	BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
	[flatten]
	if (TerrainBrush.Type == 1)
	{
		if ((wPosition.x >= (TerrainBrush.Location.x - TerrainBrush.Range)) &&
			(wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
			(wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
			(wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range))
			)
		{
			return TerrainBrush.Color;
		}
	}

	[flatten]
	if (TerrainBrush.Type == 2)
	{
		float dx = wPosition.x - TerrainBrush.Location.x;
		float dz = wPosition.z - TerrainBrush.Location.z;
		float dist = sqrt(dx * dx + dz * dz);

		if (dist <= (float)TerrainBrush.Range)
		{
			return TerrainBrush.Color;
		}
	}

	return float4(0, 0, 0, 0);
}


// --
// Line
// --

struct LineDesc
{
	float4 Color;
	uint Visible;
	float Thickness;
	float Size;
};

cbuffer CB_TerrainLine
{
	LineDesc TerrainLine;
};

float4 GetLineColor(float3 wPosition)
{
	[flatten]
	if (TerrainLine.Visible < 1)
		return float4(0, 0, 0, 0);

	float2 grid = wPosition.xz;
	float thick = TerrainLine.Thickness;

	// hlsl toos
	grid = frac(grid + thick * 0.5f); // 소수만 남기는 함수

	[flatten]
	if (grid.x < thick ||
		grid.y < thick)
		return TerrainLine.Color;

	return float4(0, 0, 0, 0);
}

