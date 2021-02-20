#include "00_Global.fx"
#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"
#include "00_EnvCubeMap.fx"

#include "00_TerrainBrush.fx"
#include "00_TerrainLayer.fx"


// --
// TerrainInput
// --

struct TerrainInput
{
	float4 Position : Position0;
	float3 Normal : Normal0;
	float2 Uv : Uv0;
	float4 Color : Color;
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

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
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
	float4 Color : Color;

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
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

	output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]);
	output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]);
	output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]);
	output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);

	output.Clipping = float4(0, 0, 0, 0);
	output.Clipping.x = dot(float4(output.wPosition, 1), Clipping);

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
	output.Color = input.Color;

	output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]);
	output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]);
	output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]);
	output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);

	output.Clipping = float4(0, 0, 0, 0);
	output.Clipping.x = dot(float4(output.wPosition, 1), Clipping);

	return output;
}


// --
// PS
// --

float4 PS_Terrain(TerrainOutput input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = GetLayerColor(input.Uv);

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
	Material.Diffuse = GetLayerColor(input.Uv);

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
	Material.Diffuse = GetLayerColor(input.Uv);

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
	c += input.Color;

	return c;
}


// --
// EnvCube PreRender
// --

// EnvCube Shadow
struct EnvCubeOutput_Terrain
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float2 Uv : Uv0;

	float4 sPosition : Position3;

	uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

// VS - EnvCube Shadow
EnvCubeOutput_Terrain VS_PreEnvCube_Terrain(TerrainInput input)
{
	EnvCubeOutput_Terrain output;

	output.oPosition = input.Position.xyz;

	output.Position = WorldPosition(input.Position);
	output.wPosition = output.Position.xyz;

	output.Position = ViewProjection(output.Position);

	output.Normal = WorldNormal(input.Normal);
	output.Uv = input.Uv;

	VSSet_Shadow(output.sPosition, output.wPosition);

	output.TargetIndex = 0;

	return output;
}

// GS_PreEnvCube_Terrain //////////
[maxvertexcount(18)]
void GS_PreEnvCube_Terrain(triangle EnvCubeOutput_Terrain input[3], inout TriangleStream<EnvCubeOutput_Terrain> stream)
{
	int vertex = 0;
	EnvCubeOutput_Terrain output;

	[unroll(6)]
	for (int i = 0; i < 6; i++)
	{
		// SV_·»´õÅ¸°Ù [i]
		output.TargetIndex = i;

		[unroll(3)]
		for (vertex = 0; vertex < 3; vertex++)
		{
			output.Position = mul(float4(input[vertex].wPosition, 1), PreEnvCube.Views[i]);
			output.Position = mul(output.Position, PreEnvCube.Projection);

			output.oPosition = input[vertex].oPosition;
			output.wPosition = input[vertex].wPosition;
			output.sPosition = input[vertex].sPosition;
			output.Normal = input[vertex].Normal;
			output.Uv = input[vertex].Uv;

			stream.Append(output);
		}

		stream.RestartStrip();
	}
}

float4 PS_PreEnvCube_Terrain(EnvCubeOutput_Terrain input) : SV_Target0
{
	// Set Material
	float3 normal = normalize(input.Normal);
	Material.Diffuse = GetLayerColor(input.Uv);

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

	// EnvCube PreRender
	P_VGP(P4, VS_PreEnvCube_Terrain, GS_PreEnvCube_Terrain, PS_PreEnvCube_Terrain)
}
