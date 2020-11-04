float4 Color = float4(1, 1, 1, 1);

struct VertexInput
{
    float4 Position : POSITION0;
    float4 Color : Color0;
};
    
struct VertexOutput
{
    float4 Position : SV_Position;  // ������ �ȼ�ȭ
    float4 Color : Color;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = input.Position;
    output.Color = input.Color;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- ���� Ÿ�� ��ȣ
{
    return input.Color * Color;
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
        SetRasterizerState(RS);  // �ɼ� ���游 ����

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}