#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

// 원본
//float4 PS(MeshOutput input) : SV_Target
//{
//    float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv).rgb;
//    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

//    return float4(diffuse * NdotL, 1);
//}

float4 PS(MeshOutput input) : SV_Target0 // SV_Target[n] <- 랜더 타겟 번호
{
	float3 color = DiffuseMap.Sample(LinearSampler, input.Uv).rgb;

	float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	float3 diffuse = color + (NdotL - 1) * 0.5;

	diffuse = clamp(diffuse, 0, 1);
	diffuse = diffuse * 0.8 + color * 0.2;

	return float4(diffuse, 1);
}

technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
		P_VP(P1, VS_Model, PS)
		P_VP(P2, VS_Animation2, PS)

		P_RS_VP(P3, FillMode_WireFrame, VS_Mesh, PS)
		P_RS_VP(P4, FillMode_WireFrame, VS_Model, PS)
		P_RS_VP(P5, FillMode_WireFrame, VS_Animation2, PS)
}