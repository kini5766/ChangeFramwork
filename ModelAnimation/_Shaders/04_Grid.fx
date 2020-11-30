float4x4 View;
float4x4 Projection;
float4x4 World;

float4 Color = float4(1, 1, 1, 1);

struct VertexInput
{
    float4 Position : POSITION0;
};

// 레스터라이저 이후 VertexInput값들이 선형보간이 되어 VertexOutput로 만들어짐
struct VertexOutput
{
    float4 Position : SV_Position;  // 정점이 픽셀화
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- 랜더 타겟 번호
{
    return Color;
}

RasterizerState RS
{
    FillMode = WIREFRAME;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(RS);  // 옵션 변경만 가능

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}