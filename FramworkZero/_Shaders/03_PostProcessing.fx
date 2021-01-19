
// --
// Saturation (채도)
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



// >-- 패스 등록 안 함 --< //


// --
// Inverse
// --

float4 PS_Inverse(VertexOutput input) : SV_TARGET0
{
	return 1 - DiffuseMap.Sample(PointSampler, input.Uv);
}


// --
// GrayScale
// --

float4 PS_GrayScale(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float average = (c.r + c.g + c.b) * 0.333f;
	return float4(average, average, average, 1);
}


// --
// GrayScale2
// --

float4 PS_GrayScale2(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 w = float3(0.2126f, 0.7152f, 0.0722f);
	float y = dot(c.rgb, w);
	return float4(y, y, y, 1);
}


// --
// Sepia
// --

float3x3 ColorToHerculesMatrix = float3x3
(
	0.393f, 0.769f, 0.189f,  // R
	0.349f, 0.686f, 0.168f,  // G
	0.272f, 0.534f, 0.131f  // B
	);

float4 PS_Sepia(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 result = mul(ColorToHerculesMatrix, c.rgb);  // 곱하기 순서 바뀌면 전치

	return float4(result, 1.0f);
}
