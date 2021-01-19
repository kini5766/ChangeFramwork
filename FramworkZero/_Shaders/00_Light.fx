#include "00_Material.fx"

#define POINT_LIGHT_MAX_COUNT 256
#define SPOT_LIGHT_MAX_COUNT 256


// --
// Global Lighting
// --

struct LightDesc
{
	float4 Ambient;
	float4 Specular;
	float3 Direction;
	float Padding;

	float3 Position;
};

cbuffer CB_Light
{
	LightDesc GlobalLight;
};

// function
void ComputeGlobalLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
	LightingInput lighting;

	lighting.LightDir = normalize(GlobalLight.Direction);
	lighting.Normal = normal;
	lighting.NdotL = dot(normal, -lighting.LightDir);
	lighting.ViewDir = normalize(ViewPosition() - wPosition);
	GetLighting(output, lighting);

	output.Ambient *= GlobalLight.Ambient;
	output.Specular *= GlobalLight.Specular;
}


// --
// Point Lighting
// --

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;

	float3 Position;
	float Range;

	float Intensity;
	float3 Padding;
};

cbuffer CB_PointLights
{
	uint PointLightCount;
	float3 CB_PointLights_Padding;

	PointLight PointLights[POINT_LIGHT_MAX_COUNT];
};

// function
void ComputePointLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
	output = (MaterialDesc)0;
	MaterialDesc result = (MaterialDesc)0;

	LightingInput lighting;
	lighting.Normal = normal;

	for (uint i = 0; i < PointLightCount; i++)
	{
		lighting.LightDir = wPosition - PointLights[i].Position;
		float dist = length(lighting.LightDir);

		[flatten]
		if (dist > PointLights[i].Range)
			continue;

		lighting.LightDir /= dist;

		// Lighting
		{
			lighting.NdotL = dot(normal, -lighting.LightDir);
			lighting.ViewDir = normalize(ViewPosition() - wPosition);
			GetLighting(result, lighting);

			result.Ambient *= PointLights[i].Ambient;
			result.Diffuse *= PointLights[i].Diffuse;
			result.Specular *= PointLights[i].Specular;
			result.Emissive *= PointLights[i].Emissive;
		}

		// Add result
		{
			float temp = PointLights[i].Range / dist;
			float att = temp * temp * PointLights[i].Intensity;

			output.Ambient += result.Ambient * temp;
			output.Diffuse += result.Diffuse * att;
			output.Specular += result.Specular * att;
			output.Emissive += result.Emissive * att;
		}
	}
}


// --
// Spot Lighting
// --

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;

	float3 Position;
	float Range;

	float3 Direction;
	float Angle;

	float Intensity;
	float3 Padding;
};

cbuffer CB_SpotLights
{
	uint SpotLightCount;
	float3 CB_SpotLights_Padding;

	SpotLight SpotLights[SPOT_LIGHT_MAX_COUNT];
};

// function
void ComputeSpotLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
	output = (MaterialDesc)0;
	MaterialDesc result = (MaterialDesc)0;

	LightingInput lighting;
	lighting.Normal = normal;

	for (uint i = 0; i < SpotLightCount; i++)
	{
		lighting.LightDir = wPosition - SpotLights[i].Position;
		float dist = length(lighting.LightDir);

		[flatten]
		if (dist > SpotLights[i].Range)
			continue;

		lighting.LightDir /= dist;

		// Lighting
		{
			lighting.NdotL = dot(normal, -lighting.LightDir);
			lighting.ViewDir = normalize(ViewPosition() - wPosition);
			GetLighting(result, lighting);

			result.Ambient *= SpotLights[i].Ambient;
			result.Diffuse *= SpotLights[i].Diffuse;
			result.Specular *= SpotLights[i].Specular;
			result.Emissive *= SpotLights[i].Emissive;
		}

		// Add result
		{
			float temp = pow(saturate(dot(lighting.LightDir, SpotLights[i].Direction)), max(0.1e-6f, 90.0f - SpotLights[i].Angle));
			float att = temp * (1.0f / max((1.0f - SpotLights[i].Intensity), 0.1e-6f));

			output.Ambient += result.Ambient * temp;
			output.Diffuse += result.Diffuse * att;
			output.Specular += result.Specular * att;
			output.Emissive += result.Emissive * att;
		}
	}
}


// --
// Burnt Lighting
// --

// wPosition이 이 큐브 속에 있으면 DirectionalLight * 0.0f
struct BurntLightDesc
{
	float3 Position;
	float Padding;

	float3 Scale;
	float Padding2;
};

cbuffer CB_BurntLight
{
	BurntLightDesc BurntLight;
};

void ComputeBurntLight(inout MaterialDesc directional, float3 wPosition)
{
	[flatten]
	if (all(BurntLight.Scale) == false)
		return;

	float3 dir = abs(wPosition - BurntLight.Position);
	float3 halfScale = BurntLight.Scale * 0.5f - 1.401298E-5f;

	if (dir.x < halfScale.x &&
		dir.y < halfScale.y &&
		dir.z < halfScale.z)
	{
		directional.Ambient = float4(0, 0, 0, 0);
		directional.Diffuse = float4(0, 0, 0, 0);
		directional.Specular = float4(0, 0, 0, 0);
		directional.Emissive = float4(0, 0, 0, 0);
	}
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
