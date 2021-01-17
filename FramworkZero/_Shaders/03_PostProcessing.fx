#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

struct VertexOutput
{
	float4 Position : SV_POSITION0;
	float2 Uv : Uv0;
};

VertexOutput VS(float4 position : POSITION0)
{
	VertexOutput output;

	output.Position = position;
	output.Uv.x = (position.x + 1) * 0.5f;
	output.Uv.y = (1 - position.y) * 0.5f;

	return output;
}

// --
// Diffuse
// --

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
	return DiffuseMap.Sample(PointSampler, input.Uv);
}

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


// --
// Saturation
// --

// --
// Sharpness
// --

// --
// Wiggle
// --

// --
// Vignette
// --

// --
// Interace
// --

// --
// LensDistortion
// --

technique11 T0
{
	P_DSS_VP(P0, DepthEnable_False, VS, PS_Diffuse)
	//P_VP(P0, VS, PS_Diffuse)
	P_VP(P1, VS, PS_Inverse)
	P_VP(P2, VS, PS_GrayScale)
	P_VP(P3, VS, PS_GrayScale2)
}