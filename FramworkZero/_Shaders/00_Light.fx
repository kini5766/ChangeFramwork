#include "00_Material.fx"

// --
// Texture Map
// --



// --
// Function
// --

void Texture(inout float4 color, Texture2D t, float2 uv, SamplerState samp)
{
	float4 c = t.Sample(samp, uv);

	// any : 1개라도 0이 아니면 true
	// pixlr으로 000색을 조정->레벨->최소1로 바꿔야함 ㅠㅠ
	[flatten]
	if (any(c.rgb)) 
		color *= c;
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
	Texture(color, t, uv, PointSampler);
}


// --
// NormalMapping
// --

void NormalMapping(float2 uv, float3 normal, float3 tangent, SamplerState samp)
{
	float4 map = NormalMap.Sample(samp, uv);

	[flatten]
	if (any(map.rgb) == false)
		return;

	// 노멀맵 -> 방향
	float3 normalPixel = map.rgb * 2.0f - 1.0f;

	// 탄젠트 공간
	float3 N = normalize(normal);
	// 직교가 아닐 수 있으니 직교화 (그람슈미트)
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	normalPixel = mul(normalPixel, TBN);
	normal = normalPixel;
}

void NormalMapping(float2 uv, inout float3 normal, float3 tangent)
{
	NormalMapping(uv, normal, tangent, PointSampler);
}


// --
// Lighting_All
// --

float4 Lighting_All(float3 normal, float3 wPosition)
{
	MaterialDesc output = (MaterialDesc)0;
	MaterialDesc result = (MaterialDesc)0;

	ComputeGlobalLight(output, normal, wPosition);
	ComputeBurntLight(output, wPosition);
	AddMaterial(result, output);

	ComputePointLight(output, normal, wPosition);
	AddMaterial(result, output);

	ComputeSpotLight(output, normal, wPosition);
	AddMaterial(result, output);

	return float4(MaterialToColor(result), 1.0f);
}


// --
// Lighting_Min
// --

float4 Lighting_Min(float3 normal)
{
	// Light Direction
	float3 L = normalize(GlobalLight.Direction);

	// Light Direction dot Normal
	float NdotL = dot(normal, -L);

	return GetDiffuse(abs(NdotL * 0.125f));
}


// --
// Lighting_MeshOutput
// --

struct MeshOutput
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;
};

float4 Lighting_MeshOutput(MeshOutput input)
{
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	float4 c = Lighting_All(normal, input.wPosition);


	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}


// --
// Lighting_MeshOutput_Shadow
// --

// output (최대 요소 개수 32개)
struct MeshOutput_Shadow
{
	float4 Position : SV_Position0;  // 레스터 라이징 위치
	float3 oPosition : Position1;  // NDC
	float3 wPosition : Position2;  // World
	//float4 wvpPosition : Position3;  // 그림자
	float4 wvpPosition_Sub[PROJECTION_TEXTURE_MAX_COUNT] : Position4;  // 프로젝션텍스쳐
	float4 sPosition : PositionS;  // 빛 기준 위치

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;
	float4 Color : Color0;

	//uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

float4 Lighting_MeshOutput_Shadow(MeshOutput_Shadow input)
{
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	float4 c = Lighting_All(normal, input.wPosition);
	PSSet_ProjectionTexture(input.wvpPosition_Sub, c);
	PSSet_Shadow(input.sPosition, c);

	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

float4 Lighting_MeshOutput_ProjectionTexture(MeshOutput_Shadow input)
{
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	float4 c = Lighting_All(normal, input.wPosition);
	PSSet_ProjectionTexture(input.wvpPosition_Sub, c);

	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}