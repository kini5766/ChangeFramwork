#include "00_Global.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target0 // SV_Target[n] <- 랜더 타겟 번호
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);
    
    // clip은 픽셀을 폐기해버림 (OM으로 넘기지 않음)
    // clip은 계산량 많음(잘 안 씀)
    // cull은 면을 폐기해버림
    clip(color.a - 0.5f);
    
    // 기본은 브론치[branth]
    //[flatten]
    //if (color.a < 0.5f)
    //// discard는 폐기
    //    discard;
    
    return color;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
}