
// --
// Brush
// --

struct BrushDesc
{
	float4 Color;
	float3 Location;
	uint Shape;
	uint Range;

	float Rad;
	float DragLength;
	//float Padding;
};

cbuffer CB_TerrainBrush
{
	BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
	[flatten]
	if (TerrainBrush.Shape == 1)
	{
		float rad = TerrainBrush.Rad;  // 사각형의 회전량 (라디안)
		float radCos = cos(rad);
		float radSin = sin(rad);

		// 점과 사각형 중점과의 사이 벡터
		float2 destance;
		destance.x = wPosition.x - TerrainBrush.Location.x;
		destance.y = wPosition.z - TerrainBrush.Location.z;

		// 벡터 회전 
		// cos(A-B) = cosA * cosB + sinA * sinB
		float dx = destance.x * radCos + destance.y * radSin;
		// sin(A-B) = sinA * cosB - cosA * sinB
		float dy = destance.y * radCos - destance.x * radSin;

		[flatten]
		if ((dx >= TerrainBrush.Range * -1.0f) &&
			(dx <= TerrainBrush.Range) &&
			(dy >= TerrainBrush.Range * -1.0f) &&
			(dy <= TerrainBrush.Range))
		{
			// 사각형 반경 안에 들어감
			return TerrainBrush.Color;
		}
	}

	[flatten]
	if (TerrainBrush.Shape == 2)
	{
		// 원
		float dx = wPosition.x - TerrainBrush.Location.x;
		float dz = wPosition.z - TerrainBrush.Location.z;
		float dist = dx * dx + dz * dz;

		[flatten]
		if (dist <= float(TerrainBrush.Range * TerrainBrush.Range))
		{
			return TerrainBrush.Color;
		}
	}

	[flatten]
	if (TerrainBrush.Shape == 3)
	{
		float length = TerrainBrush.DragLength;
		float range = TerrainBrush.Range;
		float rad = TerrainBrush.Rad;
		float radCos = cos(rad);
		float radSin = sin(rad);

		// 점과 사각형 중점과의 사이 벡터
		float2 destance;
		destance.x = wPosition.x - TerrainBrush.Location.x;
		destance.y = wPosition.z - TerrainBrush.Location.z;

		// 벡터 회전 
		// cos(A-B) = cosA * cosB + sinA * sinB
		float dx = destance.x * radCos + destance.y * radSin;
		// sin(A-B) = sinA * cosB - cosA * sinB
		float dy = destance.y * radCos - destance.x * radSin;

		if ((dx >= length * -1.0f) &&
			(dx <= 0.0f) &&
			(dy >= range * -1.0f) &&
			(dy <= range))
		{
			// 사각형 반경 안에 들어감
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

