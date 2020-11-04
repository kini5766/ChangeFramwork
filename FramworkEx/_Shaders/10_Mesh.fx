#include "00_Global.fx"
#include "00_Light.fx"

// 레스터라이저 이후 VertexTextureNormal값들이 선형보간이 되어 VertexOutput로 만들어짐
struct VertexOutput
{
    float4 Position : SV_Position;  // 정점이 픽셀화
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

VertexOutput VS(VertexTextureNormal input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- 랜더 타겟 번호
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    float3 inverse = (1 - diffuse.rgb);
    float alpha = diffuse.a;
    
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    // 자신의 색깔끼리는 감쇠 곱하기
    // 스펙큘러 더하기 (안에서는곱하기)
    
    return float4(NdotL - inverse, alpha);
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(FillMode_WireFrame); // 옵션 변경만 가능

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}