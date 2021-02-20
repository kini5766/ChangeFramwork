

// --
// PreEnvCube
// --

struct PreEnvCubeDesc
{
	matrix Views[6];
	matrix Projection;
};

cbuffer CB_PreEnvCube
{
	PreEnvCubeDesc PreEnvCube;
};


// --
// EnvCube
// --

TextureCube EnvCubeMap;
struct EnvCubeDesc
{
	uint Type;
	float Alpha;
	float RefractAmount;
	float Padding;

	// ÇÁ¸®³Ú
	float FresnelAmount;
	float FresnelBias;
	float FresnelScale;
	float Padding2;
};

cbuffer CB_EnvCube
{
	EnvCubeDesc EnvCube;
};

float4 PSSet_EnvCube(float4 oriColor, float3 normal, float3 wPosition, float3 oPosition)
{
	float4 c = 0;
	float3 viewDir = normalize(ViewPosition() - wPosition);
	float3 reflection = reflect(-viewDir, normal);
	float3 refraction = refract(-viewDir, normal, EnvCube.RefractAmount);

	if (EnvCube.Type == 0)
	{
		c = EnvCubeMap.Sample(LinearSampler, oPosition);
		c.a = EnvCube.Alpha;
	}
	else if (EnvCube.Type == 1)
	{
		c = EnvCubeMap.Sample(LinearSampler, reflection);
		c.a = EnvCube.Alpha;
	}
	else if (EnvCube.Type == 2)
	{
		c = EnvCubeMap.Sample(LinearSampler, -refraction);
		c.a = EnvCube.Alpha;
	}
	else if (EnvCube.Type == 3)
	{
		c = EnvCubeMap.Sample(LinearSampler, reflection);

		// Fresnel
		float4 fresnel = EnvCube.FresnelBias + (1.0f - EnvCube.FresnelScale) * pow(abs(1.0f - dot(-viewDir, normal)), EnvCube.FresnelAmount);
		c = EnvCube.FresnelAmount * oriColor + lerp(oriColor, c, fresnel);

		c.a = EnvCube.Alpha;
	}
	else if (EnvCube.Type == 4)
	{
		c = EnvCubeMap.Sample(LinearSampler, reflection);
		float3 clerp = lerp(c.rgb, oriColor.rgb, EnvCube.FresnelAmount);

		c.rgb = clerp;
		c.a = EnvCube.Alpha;
	}
	else if (EnvCube.Type == 5)
	{
		c = EnvCubeMap.Sample(LinearSampler, reflection);

		float3 clerp = lerp(c.rgb, oriColor.rgb, EnvCube.FresnelAmount);

		c.rgb = clerp;
		c.a = EnvCube.Alpha;
	}

	return c;
}
