Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;

// �����Ͷ����� ���� VertexInput������ ���������� �Ǿ� VertexOutput�� �������
struct VertexTerrain
{
	float4 Position : SV_Position0; // sv_ �ȼ�ȭ����
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
// ���� �˻�� �ȵ�
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
		float rad = TerrainBrush.Rad;  // �簢���� ȸ���� (����)
		float deg = rad * 57.295779513f;
		int nDeg = deg;
		float fracDeg = deg - nDeg;
		nDeg %= 90;

		deg = fracDeg + nDeg;
		rad = deg * 0.01745329252f;

		float radCos = cos(rad);
		float radSin = sin(rad);

		// ǥ���� Rect�� ����
		float right = TerrainBrush.Location.x + TerrainBrush.Range * (radCos - radSin);
		float top = TerrainBrush.Location.z + TerrainBrush.Range * (radCos + radSin);
		float left = TerrainBrush.Location.x - TerrainBrush.Range * (radCos - radSin);
		float bottom = TerrainBrush.Location.z - TerrainBrush.Range * (radCos + radSin);

		// �������� ���
		float HSlope = tan(rad);
		// ������ �������� ����
		float HBottom = bottom - HSlope * left;
		// ������ ������ ����
		float HTop= top - HSlope * right;

		// �������� ���
		float VSlope = tan(-rad);
		// ������ �������� ����
		float VLeft = left - VSlope * bottom;
		// ������ ������ ����
		float VRight = right - VSlope * top;

		[flatten]
		if (wPosition.z <= (HSlope * wPosition.x + HTop) &&
			wPosition.z >= (HSlope * wPosition.x + HBottom) &&
			wPosition.x <= (VSlope * wPosition.z + VRight) &&
			wPosition.x >= (VSlope * wPosition.z + VLeft))
		{
			// �簢�� �ݰ� �ȿ� ��
			return TerrainBrush.Color;
		}
		*/

		
		// wPosition // <- �簢�� ���� ���� �ִ��� �Ǵ��� ���
		// float3 TerrainBrush.Location //<- Rect�� ����
		// uint TerrainBrush.Range // <- Rect�� ������
		// float4 TerrainBrush.Color; // <- Rect �ݰ� �ȿ� ǥ�õ� ����
		
		float rad = TerrainBrush.Rad;  // �簢���� ȸ���� (����)
		float radCos = cos(rad);
		float radSin = sin(rad);

		// ���� �簢�� �������� ���� ����
		float2 destance;
		destance.x = wPosition.x - TerrainBrush.Location.x;
		destance.y = wPosition.z - TerrainBrush.Location.z;

		// ���� ȸ�� 
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
			// �簢�� �ݰ� �ȿ� ��
			return TerrainBrush.Color;
		}
	}

	[flatten]
	if (TerrainBrush.Shape == 2)
	{
		// ��
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

		// ���� �簢�� �������� ���� ����
		float2 destance;
		destance.x = wPosition.x - TerrainBrush.Location.x;
		destance.y = wPosition.z - TerrainBrush.Location.z;

		// ���� ȸ�� 
		// cos(A-B) = cosA * cosB + sinA * sinB
		float dx = destance.x * radCos + destance.y * radSin;
		// sin(A-B) = sinA * cosB - cosA * sinB
		float dy = destance.y * radCos - destance.x * radSin;

		if ((dx >= length * -1.0f) &&
			(dx <= 0.0f) &&
			(dy >= range * -1.0f) &&
			(dy <= range))
		{
			// �簢�� �ݰ� �ȿ� ��
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
	grid = frac(grid);  // �Ҽ����� ����� ���� �κ��� ������ �Լ�

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
	float2 range = abs(frac(grid - 0.5f) - 0.5f); // frac : �Ҽ����� ����� ���� �κ��� ������ �Լ�

	float2 speed = fwidth(grid);
	float2 pixel = range / speed;

	float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
	float4 lineColor = lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);

	return lineColor;
}

// ��������

// �ȼ����̴�����
// �б�(if�� for��) �ִ��� �� ��� �Ѵ�.
// ��� �ȼ��� �ݺ����� �� ���� ��� ������ ����
// if��� lerp�� ���
