#include "00_Light.fx"
#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"
#include "00_EnvCubeMap.fx"


// --
// MeshOutput
// --

struct MeshOutput
{
	float4 Position : SV_Position0;  // 레스터 라이징 위치
	float3 oPosition : Position1;  // NDC
	float3 wPosition : Position2;  // World

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
};

// PS_MeshOutput //////////
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

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

// PS_MeshOutput_ProjT //////////
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

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

// PS_MeshOutput_CubeMap //////////
TextureCube CubeMap;

float4 PS_CubeMap(MeshOutput input) : SV_Target0
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

	float3 viewDir = normalize(ViewPosition() - input.wPosition);

	// reflection
	float3 reflection = reflect(-viewDir, normal);
	float4 lec = CubeMap.Sample(LinearSampler, reflection);

	// Fresnel
	float r = (1.2f - 0.3f) / (1.2f / 0.3f);
	float fresnel = r + (1.0f - r) * pow(1.0f - dot(normal, viewDir), 2);
	fresnel = saturate(fresnel);

	c += lec;
	c.a = fresnel;

	return c;
}


// --
// MeshOutput_Shadow
// --

// output (최대 요소 개수 32개)
struct MeshOutput_Shadow
{
	float4 Position : SV_Position0;  // 레스터 라이징 위치
	float3 oPosition : Position1;  // NDC
	float3 wPosition : Position2;  // World
	float4 sPosition : PositionS;  // 빛 기준 위치

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
};

// PS_MeshOutput_Shadow //////////
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

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

// PS_MeshOutput_Shadow_ProjT //////////
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

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(normal);
	return max(c, cmin);
}

// PS_MeshOutput_Shadow_EnvCube //////////
float4 PS_MeshOutput_Shadow_EnvCube(MeshOutput_Shadow input) : SV_Target0
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

	// 최소 밝기 (지면 반사광)
	float4 cmin = Lighting_Min(normal);
	c = max(c, cmin);
	c.a = 1.0f;

	c = PSSet_EnvCube(c, normal, input.wPosition, input.oPosition);

	return c;
}


// --
// MeshOutput_PreEnvCube
// --
struct MeshOutput_PreEnvCube
{
	float4 Position : SV_Position0;  // 레스터 라이징 위치
	float3 oPosition : Position1;  // NDC
	float3 wPosition : Position2;  // World
	float4 sPosition : PositionS;  // 빛 기준 위치

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;

	uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

// GS_PreEnvCube_MeshOutput //////////
[maxvertexcount(18)]
void GS_PreEnvCube_MeshOutput(triangle MeshOutput_PreEnvCube input[3], inout TriangleStream<MeshOutput_PreEnvCube> stream)
{
	int vertex = 0;
	MeshOutput_PreEnvCube output;

	[unroll(6)]
	for (int i = 0; i < 6; i++)
	{
		// SV_렌더타겟 [i]
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
			output.Tangent = input[vertex].Tangent;
			output.Uv = input[vertex].Uv;

			stream.Append(output);
		}

		stream.RestartStrip();
	}
}

float4 PS_PreEnvCube_MeshOutput(MeshOutput_PreEnvCube input) : SV_Target0
{
	MeshOutput_Shadow output;
	output.Position = input.Position;
	output.oPosition = input.oPosition;
	output.wPosition = input.wPosition;
	output.sPosition = input.sPosition;
	output.Normal = input.Normal;
	output.Tangent = input.Tangent;
	output.Uv = input.Uv;
	output.Culling = float4(0, 0, 0, 0);
	output.Clipping = float4(0, 0, 0, 0);
	return PS_MeshOutput_Shadow(output);


	//Texture(Material.Diffuse, DiffuseMap, input.Uv);
	//return Material.Diffuse;
}
