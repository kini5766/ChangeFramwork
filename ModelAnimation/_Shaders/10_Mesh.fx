#include "00_Global.fx"
#include "00_Light.fx"

// �����Ͷ����� ���� VertexTextureNormal������ ���������� �Ǿ� VertexOutput�� �������
struct VertexOutput
{
    float4 Position : SV_Position;  // ������ �ȼ�ȭ
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

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- ���� Ÿ�� ��ȣ
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    float3 inverse = (1 - diffuse.rgb);
    float alpha = diffuse.a;
    
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    // �ڽ��� ���򳢸��� ���� ���ϱ�
    // ����ŧ�� ���ϱ� (�ȿ����°��ϱ�)
    
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
        SetRasterizerState(FillMode_WireFrame); // �ɼ� ���游 ����

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}