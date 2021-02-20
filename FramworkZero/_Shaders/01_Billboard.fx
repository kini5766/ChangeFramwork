#include "00_Global.fx"
#include "00_EnvCubeMap.fx"

// --
// Geometry Billboard
// --
Texture2DArray BillboardMaps;

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

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
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

		output.Culling.x = dot(position[i], Culling[0]);
		output.Culling.y = dot(position[i], Culling[1]);
		output.Culling.z = dot(position[i], Culling[2]);
		output.Culling.w = dot(position[i], Culling[3]);
		
		output.Clipping = float4(0, 0, 0, 0);
		output.Clipping.x = dot(position[i], Clipping);

		stream.Append(output);
	}
}

float4 PS_Billboard(GeometryOutput_Billboard input) : SV_TARGET0
{
	return BillboardMaps.Sample(PointSampler, float3(input.Uv, input.MapIndex));
}


// --
// PreEnvCube Billboard
// --

// GS Output -> RS
struct GeometryOutput_Billboard_PreEnvCube
{
	float4 Position : SV_POSITION0;
	float2 Uv : UV0;
	uint MapIndex : MAPINDEX0;

	uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

// GS (기본 토폴로지 : Strip)
[maxvertexcount(24)]
void GS_PreEnvCube_Billboard(
	point VertexOutout_Billboard input[1],
	inout TriangleStream<GeometryOutput_Billboard_PreEnvCube> stream)
{
	float3 vPosition = input[0].Position.xyz;
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

	GeometryOutput_Billboard_PreEnvCube output;
	[unroll(6)]
	for (int env = 0; env < 6; env++)
	{
		// SV_렌더타겟 [i]
		output.TargetIndex = env;

		[unroll(4)]
		for (int i = 0; i < 4; i++)
		{
			output.Position = mul(position[i], PreEnvCube.Views[env]);
			output.Position = mul(output.Position, PreEnvCube.Projection);
			output.Uv = uv[i];
			output.MapIndex = input[0].MapIndex;

			stream.Append(output);
		}

		stream.RestartStrip();
	}

}

float4 PS_PreEnvCube_Billboard(GeometryOutput_Billboard_PreEnvCube input) : SV_TARGET0
{
	return BillboardMaps.Sample(PointSampler, float3(input.Uv, input.MapIndex));
}


technique11 T0
{
	P_RS_BS_VGP(P0, CullMode_None, AlphaBlend_AlphaToCoverageEnable, VS_Billboard, GS_Billboard, PS_Billboard)
	
	// EnvCube PreRender
	P_RS_BS_VGP(P1, CullMode_None, AlphaBlend_AlphaToCoverageEnable, VS_Billboard, GS_PreEnvCube_Billboard, PS_PreEnvCube_Billboard)
}