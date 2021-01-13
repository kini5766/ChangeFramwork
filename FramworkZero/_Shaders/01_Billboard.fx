

// --
// Geometry Billboard
// --

// Input From IA
struct VertexInput_Billboard
{
	float4 Position : POSITION0;
	float2 Scale :  SCALE0;
	uint MapIndex : MAPINDEX0;
};

// Output To GS
struct VertexOutout_Billboard
{
	float4 Position : POSITION0;
	float2 Scale :  SCALE0;
	uint MapIndex : MAPINDEX0;
};

VertexOutout_Billboard VS_Billboard(VertexInput_Billboard input)
{
	VertexOutout_Billboard output;
	//output.Position = WorldPosition(input.Position);
	output.Position = input.Position;
	output.Scale = input.Scale;
	output.MapIndex = input.MapIndex;

	return output;
}

// GS Output -> RS
struct GeometryOutput_Billboard
{
	float4 Position : SV_POSITION0;
	float2 Uv : UV0;
	uint MapIndex : MAPINDEX0;
};

// GS (기본 토폴로지 : Strip)
// point : 정점 1개씩 받기
// inout TriangleStream : 리턴해줌
// maxvertexcount : stream 배열 최대 개수
[maxvertexcount(4)]
void GS_Billboard(
	point VertexOutout_Billboard input[1],
	inout TriangleStream<GeometryOutput_Billboard> stream)
{
	//float4 vPosition = WorldPosition(input.Position);
	float3 vPosition = input[0].Position.xyz;
	//float3 viewPos = ViewPosition();
	float3 forward = float3(0, 0, 1);
	forward = ViewInverse._31_32_33;
	float3 up = float3(0, 1, 0);
	float3 right = cross(up, forward);

	float2 halfSize = input[0].Scale * 0.5f;
	float4 position[4];
	position[0] = float4(vPosition - halfSize.x * right - halfSize.y * up, 1);  // 좌하
	position[1] = float4(vPosition - halfSize.x * right + halfSize.y * up, 1);  // 좌상
	position[2] = float4(vPosition + halfSize.x * right - halfSize.y * up, 1);  // 우하
	position[3] = float4(vPosition + halfSize.x * right + halfSize.y * up, 1);  // 우상
	float2 uv[4] = {
		float2(0, 1),
		float2(0, 0),
		float2(1, 1),
		float2(1, 0)
	};

	GeometryOutput_Billboard output;
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		output.Position = ViewProjection(position[i]);
		output.Uv = uv[i];
		output.MapIndex = input[0].MapIndex;
		stream.Append(output);
	}
}

Texture2DArray BillboardMaps;
float4 PS_Billboard(GeometryOutput_Billboard input) : SV_TARGET0
{
	return BillboardMaps.Sample(PointSampler, float3(input.Uv, input.MapIndex));
}