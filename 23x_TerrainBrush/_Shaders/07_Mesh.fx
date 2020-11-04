#include "00_Light.fx"

float4x4 View;
float4x4 Projection;
float4x4 World;
float4 Color;

struct VertexInput
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
};

// 레스터라이저 이후 VertexInput값들이 선형보간이 되어 VertexOutput로 만들어짐
struct VertexOutput
{
    float4 Position : SV_Position;  // 정점이 픽셀화
    float3 Normal : NORMAL0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = mul(input.Normal, (float3x3) World);
    
    return output;
}

// 원본
//float4 PS(VertexOutput input) : SV_Target0
//{
//    float3 diffuse = Color;

//    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

//    return float4(diffuse * NdotL, 1);
//}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- 랜더 타겟 번호
{
    float3 diffuse = (1 - Color.rgb);
    
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	//int Posterizing = NdotL * 2;

	//NdotL = Posterizing;
	//NdotL = NdotL / 2 + 0.5;
    
    // 자신의 색깔끼리는 감쇠 곱하기
    // 스펙큘러 더하기 (안에서는곱하기)
    return float4(NdotL - diffuse, Color.a);
    //return float4(Color);
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