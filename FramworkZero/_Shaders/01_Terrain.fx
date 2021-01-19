#include "00_Global.fx"
#include "00_Light.fx"
#include "00_ProjectionTexture.fx"

// 추후 추가
//#include "00_Shadow.fx"


// --
// Terrain Values
// --

Texture2D BaseMap;


// --
// Vertex In Out
// --

struct TerrainInput
{
	float4 Position : Position0;
	float3 Normal : Normal0;
	float2 Uv : Uv0;
};

struct TerrainOutput
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float2 Uv : Uv0;
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

	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);

	return c;
}

float4 PS_Terrain_ProjT(TerrainOutput input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = BaseMap.Sample(PointSampler, input.Uv);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// ProjectionTexture
	float4 wvpPosition_Sub[PROJECTION_TEXTURE_MAX_COUNT];
	VSSet_ProjectionTexture(wvpPosition_Sub, input.wPosition);
	PSSet_ProjectionTexture(wvpPosition_Sub, c);

	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
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
}
