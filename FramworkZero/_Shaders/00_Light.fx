// --
// Texture Map
// --

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

// --
// Global Light
// --

struct LightDesc
{
	float4 Ambient;
	float4 Specular;
	float3 Direction;
	float LightDesc_Padding;

	float3 Position;
};

cbuffer CB_Light
{
	LightDesc GlobalLight;
};

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
	[flatten]
	if (any(c.rgb)) color *= c;
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
	Texture(color, t, uv, PointSampler);
}

float3 MaterialToColor(MaterialDesc result)
{
	return float4(result.Ambient + result.Diffuse + result.Specular + result.Emissive).rgb;
}


// --
// Compute Function
// --

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

	//[flatten]
	if (NdotL > 0.0f)
	{
		// Diffuse
		output.Diffuse = Material.Diffuse * NdotL * 1.25f;

		// Specular
		[flatten]
		if (Material.Specular.a > 0.0f)
		{
			// 반사 벡터
			float3 R = normalize(reflect(L, N));
			float RdotE = saturate(dot(R, E));

			// 정구
			float specular = pow(RdotE, Material.Specular.a);

			output.Specular = Material.Specular * specular * GlobalLight.Specular;
		}
	}
	else
	{
		output.Diffuse = Material.Diffuse * -NdotL * 0.125f;
	}


	// Emissive
	if (Material.Emissive.a > 0.0f)
	{
		float NdotE = dot(N, E);

		float emissive = smoothstep(1.0f - Material.Emissive.a, 1.0f, 1.0f - NdotE);
		//output.Emissive = Material.Emissive * emissive;
		//emissive = saturate(floor(1.0f - NdotE + Material.Emissive.a));

		//float h = NdotE * 0.5f + 0.5f;
		float h = NdotE * 0.5f + 0.5f;
		float r = saturate(h);
		float b = saturate(1.0f - h);
		float4 c = float4(r, 0.0f, b, 1.0f);
		output.Emissive = Material.Emissive * emissive * c;
	}
}

void NormalMapping(float2 uv, inout float3 normal, float3 tangent, SamplerState samp)
{
	float3 map = NormalMap.Sample(samp, uv);

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
	//Material.diffuse *= saturate(dot(normalPixel, -GlobalLight.Direction));
}
void NormalMapping(float2 uv, inout float3 normal, float3 tangent)
{
	NormalMapping(uv, normal, tangent, PointSampler);
}