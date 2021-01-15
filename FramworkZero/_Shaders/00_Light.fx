
#define POINT_LIGHT_MAX_COUNT 256
#define SPOT_LIGHT_MAX_COUNT 256

// --
// Texture Map
// --

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

TextureCube SkyCubeMap;

// --
// Material
// --

struct MaterialDesc
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
};

cbuffer CB_Material
{
	MaterialDesc Material;
};


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

float3 MaterialToColor(MaterialDesc result)
{
	return float4(result.Ambient + result.Diffuse + result.Specular + result.Emissive).rgb;
}

void AddMaterial(inout MaterialDesc result, MaterialDesc val)
{
	result.Ambient += val.Ambient;
	result.Diffuse += val.Diffuse;
	result.Specular += val.Specular;
	result.Emissive += val.Emissive;
}


// --
// NormalMapping
// --

void NormalMapping(float2 uv, inout float3 normal, float3 tangent, SamplerState samp)
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
	//Material.Diffuse *= saturate(dot(normalPixel, -GlobalLight.Direction));
}

void NormalMapping(float2 uv, inout float3 normal, float3 tangent)
{
	NormalMapping(uv, normal, tangent, PointSampler);
}


// --
// Material
// --

float4 GetDiffuse(float NdotL, float diffuse)
{
	return Material.Diffuse * (NdotL * diffuse * Material.Diffuse.a);
}

float4 GetDiffuse(float NdotL, float4 diffuse)
{
	return Material.Diffuse * NdotL * diffuse;
}

float4 GetSpecular(float3 L, float3 N, float3 E, float4 specular)
{
	// 반사 벡터
	float3 R = normalize(reflect(L, -N));
	float RdotE = saturate(dot(R, E));

	// 정구
	float spow = saturate(pow(RdotE, Material.Specular.a));

	return Material.Specular * spow * specular;
}

float4 GetEmissive(float3 N, float3 E)
{
	float NdotE = dot(N, E);

	float eSthep = smoothstep(1.0f - Material.Emissive.a, 1.0f, 1.0f - NdotE);
	return Material.Emissive * eSthep;

	//float h = saturate(NdotE);
	//float r = saturate(1.0f - h);
	//float g = saturate(1.0f - h * 2.0f);
	//float b = saturate(1.0f - h * 2.0f) + saturate(h * 2.0f - 1.0f);
	//float4 c = float4(r, g, b, 1.0f);
	//return Material.Emissive * eSthep * c;
}

float4 GetEmissive(float3 N, float3 E, float4 emissive)
{
	return GetEmissive(N, E) * emissive;
}

// --
// Compute Function
// --


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
void ComputeMaterial(out MaterialDesc output, float3 normal, float3 wPosition)
{
	output = (MaterialDesc)0;

	output.Ambient = GlobalLight.Ambient * Material.Ambient;

	// Light Direction
	float3 L = normalize(GlobalLight.Direction);

	// Normal
	float3 N = normalize(normal);

	// Light Direction dot Normal
	float NdotL = dot(N, -L);

	// Eye
	float3 E = normalize(ViewPosition() - wPosition);

	if (NdotL > 0.0f)
	{
		// Diffuse
		output.Diffuse = GetDiffuse(NdotL, 1.0f);

		// Specular
		[flatten]
		if (Material.Specular.a > 0.0f)
		{
			output.Specular = GetSpecular(L, N, E, GlobalLight.Specular);
		}
	}
	else
	{
		output.Diffuse = GetDiffuse(-NdotL, 0.25f);
	}

	// Emissive
	if (Material.Emissive.a > 0.0f)
	{
		output.Emissive = GetEmissive(N, E);
	}
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

	//[unroll]
	for (uint i = 0; i < PointLightCount; i++)
	{
		//float3 light = PointLights[i].Position - wPosition;
		float3 L = wPosition - PointLights[i].Position;
		float dist = length(L);

		[flatten]
		if (dist > PointLights[i].Range)
			continue;

		L /= dist;

		float3 E = normalize(ViewPosition() - wPosition);
		float3 N = normalize(normal);
		float NdotL = dot(N, -L);

		// Ambient
		result.Ambient = PointLights[i].Ambient * Material.Ambient;

		[flatten]
		if (NdotL > 0.0f)
		{
			// Diffuse
			result.Diffuse = GetDiffuse(NdotL, PointLights[i].Diffuse);

			// Specular
			[flatten]
			if (Material.Specular.a > 0.0f)
			{
				result.Specular = GetSpecular(L, N, E, PointLights[i].Specular);
			}
		}

		// Emissive
		[flatten]
		if (Material.Emissive.a > 0.0f)
		{
			result.Emissive = GetEmissive(N, E, PointLights[i].Emissive);
		}

		float temp = PointLights[i].Range / dist;
		float att = temp * temp * PointLights[i].Intensity;

		output.Ambient += result.Ambient * temp;
		output.Diffuse += result.Diffuse * att;
		output.Specular += result.Specular * att;
		output.Emissive += result.Emissive * att;
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

	//[unroll]
	for (uint i = 0; i < SpotLightCount; i++)
	{
		//float3 light = SpotLights[i].Position - wPosition;
		float3 L = wPosition - SpotLights[i].Position;
		float dist = length(L);

		[flatten]
		if (dist > SpotLights[i].Range)
			continue;

		L /= dist;

		float3 E = normalize(ViewPosition() - wPosition);
		float3 N = normalize(normal);
		float NdotL = dot(N, -L);

		// Ambient
		result.Ambient = SpotLights[i].Ambient * Material.Ambient;

		[flatten]
		if (NdotL > 0.0f)
		{
			// Diffuse
			result.Diffuse = GetDiffuse(NdotL, SpotLights[i].Diffuse);

			// Specular
			[flatten]
			if (Material.Specular.a > 0.0f)
			{
				result.Specular = GetSpecular(L, N, E, SpotLights[i].Specular);
			}
		}

		// Emissive
		[flatten]
		if (Material.Emissive.a > 0.0f)
		{
			result.Emissive = GetEmissive(N, E, SpotLights[i].Emissive);
		}

		float temp = pow(saturate(dot(L, SpotLights[i].Direction)), max(0.1e-6f, 90.0f - SpotLights[i].Angle));
		float att = temp * (1.0f / max((1.0f - SpotLights[i].Intensity), 0.1e-6f));

		output.Ambient += result.Ambient * temp;
		output.Diffuse += result.Diffuse * att;
		output.Specular += result.Specular * att;
		output.Emissive += result.Emissive * att;
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
// PS All Light
// --

float4 PS_AllLight(MeshOutput input)
{
	NormalMapping(input.Uv, input.Normal, input.Tangent);
	Texture(Material.Diffuse, DiffuseMap, input.Uv);

	Texture(Material.Specular, SpecularMap, input.Uv);

	MaterialDesc output = (MaterialDesc)0;
	MaterialDesc result = (MaterialDesc)0;

	ComputeMaterial(output, input.Normal, input.wPosition);
	ComputeBurntLight(output, input.wPosition);
	AddMaterial(result, output);

	ComputePointLight(output, input.Normal, input.wPosition);
	AddMaterial(result, output);

	ComputeSpotLight(output, input.Normal, input.wPosition);
	AddMaterial(result, output);

	return float4(MaterialToColor(result), 1.0f) + input.Color;
}