#include "00_Global.fx"
#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"

#include "00_TerrainBrush.fx"

// --
// Terrain Values
// --

Texture2D BaseMap;
Texture2D Layer1AlphaMap;
Texture2D Layer1ColorMap;


// --
// TerrainInput
// --

struct TerrainInput
{
	float4 Position : Position0;
	float3 Normal : Normal0;
	float2 Uv : Uv0;

	float4 Color : Color;
	float3 Tangent : Tangent;
};


// --
// TerrainOutput
// --

struct TerrainOutput
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float2 Uv : Uv0;
};

// Shadow
struct TerrainOutput_Shadow
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float2 Uv : Uv0;

	float4 sPosition : Position3;
};


// --
// VS
// --

TerrainOutput VS_Terrain(TerrainInput input)
{
	TerrainOutput output;

	output.oPosition = input.Position.xyz;

	output.Position = WorldPosition(input.Position);
	output.wPosition = output.Position.xyz;

	output.Position = ViewProjection(output.Position);

	output.Normal = WorldNormal(input.Normal);
	output.Uv = input.Uv;

	return output;
}

// VS - Depth
DepthOutput VS_Terrain_Depth(TerrainInput input)
{
	DepthOutput output;

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}

// VS - Shadow
TerrainOutput_Shadow VS_Terrain_Shadow(TerrainInput input)
{
	TerrainOutput_Shadow output;

	output.oPosition = input.Position.xyz;

	output.Position = WorldPosition(input.Position);
	output.wPosition = output.Position.xyz;

	output.Position = ViewProjection(output.Position);

	output.Normal = WorldNormal(input.Normal);
	output.Uv = input.Uv;

	VSSet_Shadow(output.sPosition, output.wPosition);

	return output;
}


// --
// PS
// --

float4 PS_Terrain(TerrainOutput input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = BaseMap.Sample(PointSampler, input.Uv);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// ÃÖ¼Ò ¹à±â (Áö¸é ¹Ý»ç±¤)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);


	// Brush Color
	c += GetBrushColor(input.wPosition);
	c += GetLineColor(input.wPosition);

	return c;
}

float4 PS_Terrain_ProjT(TerrainOutput input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = BaseMap.Sample(PointSampler, input.Uv);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// ÃÖ¼Ò ¹à±â (Áö¸é ¹Ý»ç±¤)
	float4 cmin = Lighting_Min(normal);
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

float4 PS_Terrain_Shadow_ProjT(TerrainOutput_Shadow input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = BaseMap.Sample(PointSampler, input.Uv);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// Shadow
	c = PSSet_Shadow(input.sPosition, c);

	// ÃÖ¼Ò ¹à±â (Áö¸é ¹Ý»ç±¤)
	float4 cmin = Lighting_Min(normal);
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


// --
// Pass
// --

technique11 T0
{
	// None
	P_VP(P0, VS_Terrain, PS_Terrain)

	// Projection Texture
	P_VP(P1, VS_Terrain, PS_Terrain_ProjT)

	// Shadow - Depth
	P_RS_VP(P2, FrontCounterClockwise_True, VS_Terrain_Depth, PS_Shadow_Depth)

	// Shadow + Projection Texture
	P_VP(P3, VS_Terrain_Shadow, PS_Terrain_Shadow_ProjT)
}
