#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target0 // SV_Target[n] <- ·£´õ Å¸°Ù ¹øÈ£
{
    float4 color = BaseMap.Sample(LinearSampler, input.Uv);
    
    //float4 diffuse = (1 - color);
    
    float NdotL = dot(normalize(input.Normal), - GlobalLight.Direction);
    float3 diffuse = (1 - color.rgb);
    float4 result = float4(NdotL - diffuse, color.a);

    // Posterizing
	//NdotL = Posterizing;
	//NdotL = NdotL / 2 + 0.5;
    
    // ÀÚ½ÅÀÇ »ö±ò³¢¸®´Â °¨¼è °öÇÏ±â
    // ½ºÆåÅ§·¯ ´õÇÏ±â (¾È¿¡¼­´Â°öÇÏ±â)
    return result;
}

RasterizerState RS
{
    FillMode = WIREFRAME;
};

technique11 T0
{
    P_VP(P0, VS, PS)

    P_RS_VP(P1, FillMode_WireFrame, VS, PS)
}