#include "00_Global.fx"
#include "00_Light.fx"


//picture;

struct VertexOutput
{
	float4 Position : SV_POSITION0;
	float2 Uv : Uv0;
};


#include "03_PostProcessing.fx"
#include "03_GaussianBlur.fx"


// --
// VS
// --

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


technique11 T0
{
	// main
	P_DSS_VP(P0, DepthEnable_False, VS, PS_Diffuse)
	P_DSS_VP(P1, DepthEnable_False, VS, PS_Saturation)
	P_DSS_VP(P2, DepthEnable_False, VS, PS_Wiggle)
	P_DSS_VP(P3, DepthEnable_False, VS, PS_Vignette)
	P_DSS_VP(P4, DepthEnable_False, VS, PS_LensDistortion)
	P_DSS_VP(P5, DepthEnable_False, VS, PS_RaialBlur)

	// sub

}