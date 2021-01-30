#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"

// --
// PS_MeshOutput
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

float4 PS_MeshOutput(MeshOutput input) : SV_Target0
{
	// Set Material
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// �ּ� ��� (���� �ݻ籤)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

float4 PS_MeshOutput_ProjT(MeshOutput input) : SV_Target0
{
	// Set Material
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// ProjectionTexture
	float4 wvpPosition_Sub;
	VSSet_ProjectionTexture(wvpPosition_Sub, input.wPosition);
	PSSet_ProjectionTexture(wvpPosition_Sub, c);

	// �ּ� ��� (���� �ݻ籤)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}


// --
// Lighting_MeshOutput_Shadow
// --

// output (�ִ� ��� ���� 32��)
struct MeshOutput_Shadow
{
	float4 Position : SV_Position0;  // ������ ����¡ ��ġ
	float3 oPosition : Position1;  // NDC
	float3 wPosition : Position2;  // World
	//float4 wvpPosition : Position3;  // ���۵� ������ (���߿�)
	float4 sPosition : PositionS;  // �� ���� ��ġ

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;
	float4 Color : Color0;

	//uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

float4 PS_MeshOutput_Shadow(MeshOutput_Shadow input) : SV_Target0
{
	// Set Material
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// Shadow
	c = PSSet_Shadow(input.sPosition, c);

	// �ּ� ��� (���� �ݻ籤)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

float4 PS_MeshOutput_Shadow_ProjT(MeshOutput_Shadow input) : SV_Target0
{
	// Set Material
	float3 normal = input.Normal;
	float3 tangent = normalize(input.Tangent);

	NormalMapping(input.Uv, normal, tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	normal = normalize(normal);

	// Material Lighting
	float4 c = Lighting_All(normal, input.wPosition);

	// Shadow
	c = PSSet_Shadow(input.sPosition, c);

	// ProjectionTexture
	float4 wvpPosition_Sub;
	VSSet_ProjectionTexture(wvpPosition_Sub, input.wPosition);
	PSSet_ProjectionTexture(wvpPosition_Sub, c);

	// �ּ� ��� (���� �ݻ籤)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}