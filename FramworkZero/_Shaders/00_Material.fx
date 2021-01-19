
// --
// Material Values
// --

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

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
// Material Texture Sample
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
// Material Lighting
// --

float4 GetDiffuse(float NdotL)
{
	return Material.Diffuse * (NdotL * Material.Diffuse.a);
}

float4 GetSpecular(float3 L, float3 N, float3 viewDir)
{
	// 반사 벡터
	float3 R = normalize(reflect(L, -N));
	float RdotE = saturate(dot(R, viewDir));

	// 정구
	float spow = saturate(pow(RdotE, Material.Specular.a));

	return Material.Specular * spow;
}

float4 GetRim(float3 N, float3 viewDir, float NdotL)
{
	float rimDot = 1.0f - saturate(dot(N, viewDir));
	float rimIntensity = rimDot * NdotL;
	rimIntensity = smoothstep(1.0f - Material.Emissive.a, 1.0f, rimIntensity);
	return Material.Emissive * rimIntensity;
}


// --
// Material All Lighting
// --

struct LightingInput
{
	float3 Normal;
	float NdotL;
	float3 ViewDir;
	float3 LightDir;
};
void GetLighting(out MaterialDesc output, LightingInput input)
{
	// Ambient
	output = (MaterialDesc)0;
	output.Ambient = Material.Ambient;

	[flatten]
	if (input.NdotL > 0.0f)
	{
		// Diffuse
		output.Diffuse = GetDiffuse(input.NdotL);

		// Specular
		[flatten]
		if (Material.Specular.a > 0.0f)
			output.Specular = GetSpecular(input.LightDir, input.Normal, input.ViewDir);
	}

	// Emissive
	[flatten]
	if (Material.Emissive.a > 0.0f)
		output.Emissive = GetRim(input.Normal, input.ViewDir, input.NdotL);
}


// --
// Material Calculator
// --

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

