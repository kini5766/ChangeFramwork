#include "00_Global.fx"
#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"
#include "00_EnvCubeMap.fx"


// --
// 1Pass Result
// --

cbuffer CB_Reflection
{
	matrix ReflectionView;
};
Texture2D ReflectionMap;
Texture2D ReflectionDepthMap;

Texture2D RefractionMap;
Texture2D RefractionDepthMap;


// --
// Water
// --

Texture2D WaterMap;

SamplerState WaterSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};

struct WaterDesc
{
	float2 NormalMapTile;
	float WaveTransform;
	float WaveScale;
};


cbuffer CB_Water
{
	WaterDesc Water;
};

struct VertexInput_Water
{
	float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput_Water
{
	float4 Position : SV_Position;
	float3 wPosition : Position1;
	float4 ReflectionPosition : Position2;
	float4 RefractionPosition : Position3;

	float2 Uv : Uv;

	float2 Normal : Uv1;
	float2 Normal2 : Uv2;

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;

	uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

VertexOutput_Water VS_Water(VertexInput_Water input)
{
	VertexOutput_Water output = (VertexOutput_Water)0;

	output.Position = WorldPosition(input.Position);
	output.wPosition = output.Position.xyz;
	output.Position = ViewProjection(output.Position);

	// Reflection
	output.ReflectionPosition = WorldPosition(input.Position);
	output.ReflectionPosition = mul(output.ReflectionPosition, ReflectionView);
	output.ReflectionPosition = mul(output.ReflectionPosition, Projection);

	// Refraction
	output.RefractionPosition = output.Position;

	output.Uv = input.Uv;

	output.Normal = input.Uv * Water.NormalMapTile.x;
	output.Normal2 = input.Uv * Water.NormalMapTile.y;

	output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]);
	output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]);
	output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]);
	output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);
	
	output.Clipping = float4(0, 0, 0, 0);
	output.Clipping.x = dot(float4(output.wPosition, 1), Clipping);

	return output;
}

float WaterDepth(VertexOutput_Water input, float2 reflectionUv)
{
	float z = input.ReflectionPosition.z / input.ReflectionPosition.w;

	float depth = RefractionDepthMap.Sample(WaterSampler, reflectionUv).r;

	depth *= input.ReflectionPosition.w;
	float l = saturate(depth - input.ReflectionPosition.z) * 10.0f;
	return saturate(pow(l, 2));

	float n = 0.1f;
	float f = 1000.0f;
	//float l;
	l = depth;
	float z2 = (n * 2.0f) / (f + n - l * (f - n));
	l = z;
	float z3 = (n * 2.0f) / (f + n - l * (f - n));
	return saturate(z2 - z3);
}

float4 PS_Water(VertexOutput_Water input) : SV_Target0
{
	// Normal
	input.Normal.y += Water.WaveTransform;
	input.Normal2.x += Water.WaveTransform;

	float4 normalMap = NormalMap.Sample(LinearSampler, input.Normal) * 2.0f - 1.0f;
	float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Normal2) * 2.0f - 1.0f;
	float3 normal = normalize(normalMap.rgb + normalMap2.rgb);


	// Reflection
	float2 reflectionUv;
	reflectionUv.x = input.ReflectionPosition.x / input.ReflectionPosition.w * 0.5f + 0.5f;
	reflectionUv.y = -input.ReflectionPosition.y / input.ReflectionPosition.w * 0.5f + 0.5f;
	
	float2 reflection = reflectionUv + normal.xy * Water.WaveScale;
	float4 reflectionColor = ReflectionMap.Sample(WaterSampler, reflection);

	// Refraction
	float2 refractionUv;
	refractionUv.x = input.RefractionPosition.x / input.RefractionPosition.w * 0.5f + 0.5f;
	refractionUv.y = -input.RefractionPosition.y / input.RefractionPosition.w * 0.5f + 0.5f;
	
	float2 refraction = refractionUv + normal.xy * Water.WaveScale;
	float4 refractionColor = RefractionMap.Sample(WaterSampler, refraction);


	// WaterDepth
	input.Uv.x += Time * 0.005f;
	input.Uv.y += Time * 0.009f;
	float2 waterDepth = input.Uv + normal.xy * Water.WaveScale;
	float4 waterColor = WaterMap.Sample(LinearSampler, waterDepth);
	float d = WaterDepth(input, refraction);
	//return float4(d, d, d, 1.0f);
	refractionColor = lerp(refractionColor, waterColor, d);


	// Fresnel
	float3 viewDir = normalize(ViewPosition() - input.wPosition);
	float3 heightView = viewDir.yyy;
	float r = (1.2f - 0.3f) / (1.2f / 0.3f);
	float fresnel = r + (1.0f - r) * pow(1.0f - dot(normal, heightView), 2);
	fresnel = 1.0f - saturate((fresnel - 0.2f) * 1.3f);

	float4 c = lerp(reflectionColor, refractionColor, fresnel);


	// Material Lighting (µðÇ»Áî ÄÃ·¯ 0, 0, 0)
	Material.Diffuse = float4(0, 0, 0, 0);
	c += Lighting_All(normal, input.wPosition);

	return float4(c.rgb, 1.0f);
}

// GS_PreEnvCube_Water //////////
[maxvertexcount(18)]
void GS_PreEnvCube_Water(triangle VertexOutput_Water input[3], inout TriangleStream<VertexOutput_Water> stream)
{
	int vertex = 0;
	VertexOutput_Water output = (VertexOutput_Water)0;

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

			output.wPosition = input[vertex].wPosition;
			output.ReflectionPosition = input[vertex].ReflectionPosition;
			output.RefractionPosition = input[vertex].RefractionPosition;

			output.Uv = input[vertex].Uv;

			output.Normal = input[vertex].Normal;
			output.Normal2 = input[vertex].Normal2;
			
			stream.Append(output);
		}

		stream.RestartStrip();
	}
}


technique11 T0
{
	P_VP(P0, VS_Water, PS_Water)

	// EnvCube PreRender
	P_VGP(P1, VS_Water, GS_PreEnvCube_Water, PS_Water)
};