#include "00_Global.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target0 // SV_Target[n] <- ���� Ÿ�� ��ȣ
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);
    
    // clip�� �ȼ��� ����ع��� (OM���� �ѱ��� ����)
    // clip�� ��귮 ����(�� �� ��)
    // cull�� ���� ����ع���
    clip(color.a - 0.5f);
    
    // �⺻�� ���ġ[branth]
    //[flatten]
    //if (color.a < 0.5f)
    //// discard�� ���
    //    discard;
    
    return color;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
}