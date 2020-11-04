Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;

// 레스터라이저 이후 VertexInput값들이 선형보간이 되어 VertexOutput로 만들어짐
struct VertexTerrain
{
	float4 Position : SV_Position0; // sv_ 픽셀화기준
	float3 wPosition : POSITION1;
	float3 Normal : NORMAL0;
	float2 Uv : UV;

	//float4 Color : COLOR0;
};

// PS /////////////////////////////////////////////////////////////////////////////

float4 GetBaseColor(float2 uv)
{
	return BaseMap.Sample(LinearSampler, uv);
}

float4 GetLayerColor(float4 base, float2 uv)
{
	float alpha = AlphaMap.Sample(LinearSampler, uv).r;
	float4 layer = LayerMap.Sample(LinearSampler, uv);

	return lerp(base, layer, alpha);
	//return lerp(base, layer, 1 - alpha);
}

// VS /////////////////////////////////////////////////////////////////////////////
// 문법 검사는 안됨
VertexTerrain VS(VertexTextureNormal input)
{
	VertexTerrain output;
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3) World);
	output.Uv = input.Uv;

	//output.Color = GetBrushColor(output.wPosition);

	return output;
}

// Brush /////////////////////////////////////////////////////////////////////////////

struct BrushDesc
{
	float4 Color;
	float3 Location;
	uint Shape;
	uint Range;
	float Rad;
	float DragLength;
};

cbuffer CB_Brush
{
	BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
	[flatten]
	if (TerrainBrush.Shape == 1)
	{
		/*
		float rad = TerrainBrush.Rad;  // 사각형의 회전량 (라디안)
		float deg = rad * 57.295779513f;
		int nDeg = deg;
		float fracDeg = deg - nDeg;
		nDeg %= 90;

		deg = fracDeg + nDeg;
		rad = deg * 0.01745329252f;

		float radCos = cos(rad);
		float radSin = sin(rad);

		// 표시할 Rect의 범위
		float right = TerrainBrush.Location.x + TerrainBrush.Range * (radCos - radSin);
		float top = TerrainBrush.Location.z + TerrainBrush.Range * (radCos + radSin);
		float left = TerrainBrush.Location.x - TerrainBrush.Range * (radCos - radSin);
		float bottom = TerrainBrush.Location.z - TerrainBrush.Range * (radCos + radSin);

		// 가로축의 경사
		float HSlope = tan(rad);
		// 가로축 시작점의 높이
		float HBottom = bottom - HSlope * left;
		// 가로축 끝점의 높이
		float HTop= top - HSlope * right;

		// 세로축의 경사
		float VSlope = tan(-rad);
		// 세로축 시작점의 높이
		float VLeft = left - VSlope * bottom;
		// 세로축 끝점의 높이
		float VRight = right - VSlope * top;

		[flatten]
		if (wPosition.z <= (HSlope * wPosition.x + HTop) &&
			wPosition.z >= (HSlope * wPosition.x + HBottom) &&
			wPosition.x <= (VSlope * wPosition.z + VRight) &&
			wPosition.x >= (VSlope * wPosition.z + VLeft))
		{
			// 사각형 반경 안에 들어감
			return TerrainBrush.Color;
		}
		*/

		
		// wPosition // <- 사각형 범위 내에 있는지 판단할 대상
		// float3 TerrainBrush.Location //<- Rect의 중점
		// uint TerrainBrush.Range // <- Rect의 반지름
		// float4 TerrainBrush.Color; // <- Rect 반경 안에 표시될 색상
		
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
		if ((dx >= TerrainBrush.Range * - 1.0f) &&
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

// Line /////////////////////////////////////////////////////////////////////////////

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

float4 GetBasicLineColor(float3 wPosition)
{
	[flatten]
	if (TerrainLine.Visible < 1)
		return float4(0, 0, 0, 0);

	float2 grid = wPosition.xz;  // x <- x, y <- z
	grid /= TerrainLine.Size;
	grid = frac(grid);  // 소수점만 남기고 정수 부분은 날리는 함수

	float thick = TerrainLine.Thickness / TerrainLine.Size;

	[flatten]
	if (grid.x < thick || grid.y < thick)
		return TerrainLine.Color;

	return float4(0, 0, 0, 0);
}

float4 GetLineColor(float3 wPosition)
{
	[flatten]
	if (TerrainLine.Visible < 1)
		return float4(0, 0, 0, 0);

	float2 grid = wPosition.xz;  // x <- x, y <- z
	grid /= TerrainLine.Size;
	float2 range = abs(frac(grid - 0.5f) - 0.5f); // frac : 소수점만 남기고 정수 부분은 날리는 함수

	float2 speed = fwidth(grid);
	float2 pixel = range / speed;

	float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
	float4 lineColor = lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);

	return lineColor;
}

// 레스터택

// 픽셀세이더에서
// 분기(if문 for문) 최대한 안 써야 한다.
// 모든 픽셀이 반복문이 다 돌고 모두 끝나야 진행
// if대신 lerp를 사용
