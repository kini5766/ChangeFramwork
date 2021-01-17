
// --
// Saturation (Ã¤µµ)
// --

float Saturation = 3.0f;

float4 PS_Saturation(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 w = float3(0.2126f, 0.7152f, 0.0722f);
	float y = dot(c.rgb, w);
	c.rgb = lerp(y, c.rgb, Saturation);
	return c;
}


// --
// Wiggle
// --

float2 WiggleOffset = float2(10, 10);
float2 WiggleAmount = float2(0.01f, 0.01f);
float4 PS_Wiggle(VertexOutput input) : SV_TARGET0
{
	float2 uv = input.Uv;
	uv.x += sin(Time + uv.x * WiggleOffset.x) * WiggleAmount.x;
	uv.y += cos(Time + uv.y * WiggleOffset.y) * WiggleAmount.y;

	return DiffuseMap.Sample(PointSampler, uv);
}


// --
// Vignette
// --
float VignettePower = 2.0f;
float2 VignetteScale = float2(2.0f, 2.0f);
float4 PS_Vignette(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float radius = length((input.Uv - 0.5f) * 2.0f / VignetteScale);
	float vignette = pow(radius + 1e-6, VignettePower);

	return saturate(1 - vignette) * c;
}

// --
// LensDistortion
// --
float LensPower = 1.0f;
float3 DistortionWeight = -0.02f;

float4 PS_LensDistortion(VertexOutput input) : SV_TARGET0
{
	float2 uv = input.Uv * 2 - 1;
	float radius = uv.x * uv.x + uv.y * uv.y;
	float squaredRadius = sqrt(radius);
	float3 rho = DistortionWeight * pow(squaredRadius + 1e-6f, LensPower) + 1;
	float2 uvR = (rho.r * uv + 1) * 0.5f;
	float2 uvG = (rho.g * uv + 1) * 0.5f;
	float2 uvB = (rho.b * uv + 1) * 0.5f;

	float4 c = 0;
	c.r = DiffuseMap.Sample(PointSampler, uvR).r;
	c.g = DiffuseMap.Sample(PointSampler, uvG).g;
	c.b = DiffuseMap.Sample(PointSampler, uvB).b;
	return c;
}
