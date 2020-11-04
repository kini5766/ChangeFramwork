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
    float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv).rgb;
    float3 inverse = (1 - diffuse.rgb);
    
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);
    
    return float4(NdotL - inverse, 1);
}

technique11 T0
{
    P_VP(P0, VS_Model, PS)

    P_RS_VP(P1, FillMode_WireFrame, VS_Model, PS)
}