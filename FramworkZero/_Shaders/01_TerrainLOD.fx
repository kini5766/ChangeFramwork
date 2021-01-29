#include "00_Global.fx"
#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"

#include "00_TerrainBrush.fx"
#include "00_TerrainLayer.fx"


Texture2D HeightMap;


struct TerrainLODDesc
{
	float2 Distance;
	float2 TessFactor;

	float CellSpacing;
	float CellSpacingU;
	float CellSpacingV;
	float HeightScale;
};

cbuffer CB_TerrainLOD
{
	TerrainLODDesc TerrainLOD;
};


// --
// VS
// --

struct VertexOutput
{
	float4 Position : Position;
	float2 Uv : Uv;
};

VertexOutput VS(VertexOutput input)
{
	VertexOutput output;
	output.Position = input.Position;
	output.Uv = input.Uv;
	return output;
}


// --
// CHS + HS
// --

struct CHullOutput
{
	// 왼쪽 위 부터 반시계 방향으로 0 1 2번
	float Edge[4] : SV_TessFactor;  // 정점잇는 간선(변)
	// 정점의 무게중심
	float Inside[2] : SV_InsideTessFactor;
};

float CalcTessFactor(float3 position)
{
	float dist = distance(position, ViewPosition());
	float ratio = saturate((dist - TerrainLOD.Distance.y) / (TerrainLOD.Distance.x - TerrainLOD.Distance.y));

	return lerp(TerrainLOD.TessFactor.x, TerrainLOD.TessFactor.y, ratio);
}

CHullOutput CHS(InputPatch<VertexOutput, 4> input)
{
	float4 position[4];
	position[0] = WorldPosition(input[0].Position);
	position[1] = WorldPosition(input[1].Position);
	position[2] = WorldPosition(input[2].Position);
	position[3] = WorldPosition(input[3].Position);

	// [2] [3]
	// [0] [1]
	float3 eL = (position[0] + position[2]).xyz * 0.5f;
	float3 eB = (position[0] + position[1]).xyz * 0.5f;
	float3 eR = (position[1] + position[3]).xyz * 0.5f;
	float3 eT = (position[2] + position[3]).xyz * 0.5f;

	CHullOutput output;

	output.Edge[0] = CalcTessFactor(eL);
	output.Edge[1] = CalcTessFactor(eB);
	output.Edge[2] = CalcTessFactor(eR);
	output.Edge[3] = CalcTessFactor(eT);

	float3 center = 0;
	for (int i = 0; i < 4; i++)
		center += input[i].Position.xyz;
	center *= 0.25f;

	output.Inside[0] = CalcTessFactor(center);
	output.Inside[1] = CalcTessFactor(center);
	
	return output;
}

struct HullOutput
{
	float4 Position : Position0;
	float2 Uv : Uv;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexOutput, 4> input, uint id : SV_OutputControlPointID)
{
	HullOutput output;
	output.Position = input[id].Position;
	output.Uv = input[id].Uv;

	return output;
}


// --
// ds
// --

struct DomainOutput
{
	float4 Position : SV_Position;
	float3 wPosition : Position1;
	float2 Uv : UV;
};

[domain("quad")]
DomainOutput DS(CHullOutput input, const OutputPatch<HullOutput, 4> patch, float2 uv : SV_DomainLocation)
{
	DomainOutput output;

	float4 p1 = lerp(patch[0].Position, patch[1].Position, uv.x);
	float4 p2 = lerp(patch[2].Position, patch[3].Position, uv.x);
	float4 position = lerp(p1, p2, uv.y);
	//output.Position = position;

	float2 uv1 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
	float2 uv2 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
	float2 texCoord = lerp(uv1, uv2, uv.y);

	// Sample heightMap
	// PS아닌 곳에서도 할 수 있는 심플하는 방법 Load, SampleLevel
	position.y = HeightMap.SampleLevel(LinearSampler, texCoord, 0).r * 255.0f * TerrainLOD.HeightScale;

	output.Position = float4(position.xyz, 1);
	output.Position = WorldPosition(output.Position);
	output.wPosition = output.Position.xyz;
	output.Position = ViewProjection(output.Position);
	
	output.Uv = texCoord;

	return output;
}

// DS - Depth
[domain("quad")]
DepthOutput DS_Depth(CHullOutput input, const OutputPatch<HullOutput, 4> patch, float2 uv : SV_DomainLocation)
{
	DepthOutput output;

	float4 p1 = lerp(patch[0].Position, patch[1].Position, uv.x);
	float4 p2 = lerp(patch[2].Position, patch[3].Position, uv.x);
	float4 position = lerp(p1, p2, uv.y);
	//output.Position = position;

	float2 uv1 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
	float2 uv2 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
	float2 texCoord = lerp(uv1, uv2, uv.y);

	// Sample heightMap
	// PS아닌 곳에서도 할 수 있는 심플하는 방법 Load, SampleLevel
	position.y = HeightMap.SampleLevel(LinearSampler, texCoord, 0).r * 255.0f * TerrainLOD.HeightScale;

	output.Position = float4(position.xyz, 1);

	output.Position = WorldPosition(output.Position);
	output.Position = mul(output.Position, Shadow.View);
	output.Position = mul(output.Position, Shadow.Projection);

	output.sPosition = output.Position;;

	return output;
}

// --
// PS
// --
float4 PS(DomainOutput input) : SV_Target
{
	// CellSpacingU, V : Pixelsize = 1 / screen;
	float2 left = input.Uv + float2(-TerrainLOD.CellSpacingU, 0.0f);
	float2 right = input.Uv + float2(+TerrainLOD.CellSpacingU, 0.0f);
	float2 top = input.Uv + float2(0.0f, -TerrainLOD.CellSpacingV);
	float2 buttom = input.Uv + float2(0.0f, +TerrainLOD.CellSpacingV);

	float leftY = HeightMap.Sample(LinearSampler, left).r * 255 * TerrainLOD.HeightScale;
	float rightY = HeightMap.Sample(LinearSampler, right).r * 255 * TerrainLOD.HeightScale;
	float topY = HeightMap.Sample(LinearSampler, top).r * 255 * TerrainLOD.HeightScale;
	float bottomY = HeightMap.Sample(LinearSampler, buttom).r * 255 * TerrainLOD.HeightScale;

	float3 T = normalize(float3(TerrainLOD.CellSpacing * 2.0f, rightY - leftY, 0.0f));
	float3 B = normalize(float3(0.0f, bottomY - topY, TerrainLOD.CellSpacing * -2.0f));
	// Normal
	float3 N = normalize(cross(T, B));
	NormalMapping(input.Uv, N, B, LinearSampler);

	// Lambert
	Material.Diffuse = GetLayerColor(input.Uv);
	float4 c = Lighting_All(N, input.wPosition);

	// Shadow
	float4 sPosition;
	VSSet_Shadow(sPosition, input.wPosition);
	c = PSSet_Shadow(sPosition, c);

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(N);
	c = max(c, cmin);

	// ProjectionTexture
	float4 wvpPosition_Sub;
	VSSet_ProjectionTexture(wvpPosition_Sub, input.wPosition);
	PSSet_ProjectionTexture(wvpPosition_Sub, c);

	// Brush Color
	c += GetBrushColor(input.wPosition);
	c += GetLineColor(input.wPosition);

	return c;
}

float4 PS_Wire(DomainOutput input) : SV_Target
{
	return float4(1, 1, 1, 1);
}

technique11 T0
{
	P_VTP(P0, VS, HS, DS, PS)
	P_RS_VTP(P1, FrontCounterClockwise_True, VS, HS, DS_Depth, PS_Shadow_Depth)
	P_RS_VTP(P2, FillMode_WireFrame, VS, HS, DS, PS_Wire)
};
