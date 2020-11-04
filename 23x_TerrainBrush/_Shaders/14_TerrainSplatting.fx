#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target0 // SV_Target[n] <- 랜더 타겟 번호
{
    float4 color = GetLayerColor(GetBaseColor(input.Uv), input.Uv);
    
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);
    float3 diffuse = (1 - color.rgb);
    float4 result = float4(NdotL - diffuse, color.a);
    
    //float4 brush = input.Color;
    float4 brushColor = GetBrushColor(input.wPosition);
    float4 lineColor = GetLineColor(input.wPosition);
    
    // 조명 조합은 가산 연산으로
    return result + brushColor + lineColor;
}

technique11 T0
{
    P_VP(P0, VS, PS)

    P_RS_VP(P1, FillMode_WireFrame, VS, PS)
}