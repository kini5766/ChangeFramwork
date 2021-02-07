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

float4 PS_Water(VertexOutput_Water input) : SV_Target
{
	// Normal
	input.Normal.y += Water.WaveTransform;
	input.Normal2.x += Water.WaveTransform;

	float4 normalMap = NormalMap.Sample(LinearSampler, input.Normal) * 2.0f - 1.0f;
	float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Normal2) * 2.0f - 1.0f;
	float3 normal = normalMap.rgb + normalMap2.rgb;

	// Reflection
	float2 reflection;
	reflection.x = input.ReflectionPosition.x / input.ReflectionPosition.w * 0.5f + 0.5f;
	reflection.y = -input.ReflectionPosition.y / input.ReflectionPosition.w * 0.5f + 0.5f;
	reflection += normal.xy * Water.WaveScale;

	float4 reflectionColor = ReflectionMap.Sample(WaterSampler, reflection);
	return float4(reflectionColor.rgb, 1.0f);

	// Refraction
	float2 refraction;
	refraction.x = input.RefractionPosition.x / input.RefractionPosition.w * 0.5f + 0.5f;
	refraction.y = -input.RefractionPosition.y / input.RefractionPosition.w * 0.5f + 0.5f;
	refraction += normal.xy * Water.WaveScale;

	float4 refractionColor = RefractionMap.Sample(WaterSampler, refraction);

	// Fresnel
	float3 viewDir = normalize(ViewPosition() - input.wPosition);
	float3 heightView = viewDir.yyy;
	float r = (1.2f - 1.0f) / (1.2f / 1.0f);
	float fresnel = saturate(min(1, r + (1 - r) * pow(1 - dot(normal, heightView), 2)));

	float4 c = lerp(reflectionColor, refractionColor, fresnel);

	// Material Lighting (��ǻ�� �÷� 0, 0, 0)
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
//	float x = (Water.TerrainWidth / 2 - Water.WaterWidth / 2) / Water.TerrainWidth; //����� ������ ũ�� ���� -> ����ȭ
//	float y = (Water.TerrainHeight / 2 - Water.WaterHeight / 2) / Water.TerrainHeight;
//
//	float u = input.Uv.x / Water.TerrainWidth * Water.WaterWidth; //��ü �п� 1
//	float v = input.Uv.y / Water.TerrainHeight * Water.WaterHeight;
//	float2 uv = float2(x, y) + float2(u, v);
//
//	float height = HeightMap.Sample(LinearSampler, uv/*float2(u, v)*/).r * 255.0f / 10.0f;
//	height = saturate(Water.WaterPositionY * height / Water.WaterPositionY); //����� ������ ������ ����
//
//	float4 waterColor = WaterMap.Sample(LinearSampler, input.Uv * 0.2f/*reflection*/) * 2.0f; //���͸��� �ʹ� ��¥��
//	diffuse += (waterColor * height * 0.5f); //�ʹ� �౺
//
//	return float4(diffuse.rgb, 1 - (height * 0.75f));
//	//��������(height) ������ �������̹Ƿ� ���İ��� �پ��
//}