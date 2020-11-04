float4x4 View;
float4x4 Projection;
float4x4 World;

float4 Color = float4(1, 1, 1, 1);

struct VertexInput
{
    float4 Position : POSITION0;
};

// �����Ͷ����� ���� VertexInput������ ���������� �Ǿ� VertexOutput�� �������
struct VertexOutput
{
    float4 Position : SV_Position;  // ������ �ȼ�ȭ
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- ���� Ÿ�� ��ȣ
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
        SetRasterizerState(RS);  // �ɼ� ���游 ����

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}