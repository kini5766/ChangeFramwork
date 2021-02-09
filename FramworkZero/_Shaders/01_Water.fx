#include "00_Global.fx"
#include "00_PixelOutput.fx"


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

float4 PS_Water(VertexOutput_Water input) : SV_Target
{
	// Normal
	input.Normal.y += Water.WaveTransform;
	input.Normal2.x += Water.WaveTransform;

	float4 normalMap = NormalMap.Sample(LinearSampler, input.Normal) * 2.0f - 1.0f;
	float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Normal2) * 2.0f - 1.0f;
	float3 normal = normalMap.rgb + normalMap2.rgb;


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
	float fresnel = saturate(min(1.0f, r + (1.0f - r) * pow(1.0f - dot(normal, heightView), 16)));

	float4 c = lerp(reflectionColor, refractionColor, fresnel);


	// Material Lighting (디퓨즈 컬러 0, 0, 0)
	Material.Diffuse = float4(0, 0, 0, 0);
	c += Lighting_All(normal, input.wPosition);

	return float4(c.rgb, 1.0f);
}

technique11 T0
{
	P_VP(P0, VS_Water, PS_Water)
};

//float4 WaTerst_Depth(VertexOutput_Water input, float4 diffuse)
//{
//	float x = (Water.TerrainWidth / 2 - Water.WaterWidth / 2) / Water.TerrainWidth; //수면과 지면의 크기 차이 -> 비율화
//	float y = (Water.TerrainHeight / 2 - Water.WaterHeight / 2) / Water.TerrainHeight;
//
//	float u = input.Uv.x / Water.TerrainWidth * Water.WaterWidth; //전체 분에 1
//	float v = input.Uv.y / Water.TerrainHeight * Water.WaterHeight;
//	float2 uv = float2(x, y) + float2(u, v);
//
//	float height = HeightMap.Sample(LinearSampler, uv/*float2(u, v)*/).r * 255.0f / 10.0f;
//	height = saturate(Water.WaterPositionY * height / Water.WaterPositionY); //수면과 지면의 떨어진 정도
//
//	float4 waterColor = WaterMap.Sample(LinearSampler, input.Uv * 0.2f/*reflection*/) * 2.0f; //워터맵이 너무 통짜네
//	diffuse += (waterColor * height * 0.5f); //너무 밝군
//
//	return float4(diffuse.rgb, 1 - (height * 0.75f));
//	//높을수록(height) 수심이 얕은거이므로 알파값이 줄어듬
//}