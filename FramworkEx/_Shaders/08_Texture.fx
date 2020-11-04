float4x4 View;
float4x4 Projection;
float4x4 World;

Texture2D Texture;

struct VertexInput
{
    float4 Position : POSITION0;
    float2 UV : UV0;
};

// �����Ͷ����� ���� VertexInput������ ���������� �Ǿ� VertexOutput�� �������
struct VertexOutput
{
    float4 Position : SV_Position;  // ������ �ȼ�ȭ
    float2 UV : UV0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.UV = input.UV;
    
    return output;
}

SamplerState sSampler;
float4 PS(VertexOutput input) : SV_Target0 // SV_Target[n] <- ���� Ÿ�� ��ȣ
{
    float4 pxiel = Texture.Sample(sSampler, input.UV);
    
    return float4(pxiel);
}

// Address --------------------------------------------------------------------

SamplerState Sampler_AddressWrap
{
    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState Sampler_MIRROR
{
    AddressU = MIRROR;
    AddressV = MIRROR;
};

SamplerState Sampler_CLAMP
{
    AddressU = CLAMP;
    AddressV = CLAMP;
};

SamplerState Sampler_BORDER
{
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = float4(0.36, 0.88, 0.12, 1);
};

SamplerState Sampler_MIRROR_ONCE
{
    AddressU = MIRROR_ONCE;
    AddressV = MIRROR_ONCE;
};

uint Address;

float4 PS_Address(VertexOutput input) : SV_Target0
{
    [branch]  // �⺻���� �귱ġ ����
    switch (Address)
    {
    case 0: return Texture.Sample(Sampler_AddressWrap, input.UV);
    case 1: return Texture.Sample(Sampler_MIRROR, input.UV);
    case 2: return Texture.Sample(Sampler_CLAMP, input.UV);
    case 3: return Texture.Sample(Sampler_BORDER, input.UV);
    };
    
    return float4(0, 0, 0, 1);
}

// Filter ---------------------------------------------------------------------
SamplerState Sampler_Filter_Point
{
    Filter = MIN_MAG_MIP_POINT;
};
SamplerState Sampler_Filter_LINEAR
{
    Filter = MIN_MAG_MIP_LINEAR;
};

uint Filter;
float4 PS_Filter(VertexOutput input) : SV_Target0
{
    [branch]  // �⺻���� �귱ġ ����
    switch (Filter)
    {
        case 0:
            return Texture.Sample(sSampler, input.UV);
        case 1:
            return Texture.Sample(Sampler_Filter_Point, input.UV);
        case 2:
            return Texture.Sample(Sampler_Filter_LINEAR, input.UV);
    };
    
    return float4(0, 0, 0, 1);
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
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Address()));
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Filter()));
    }
}