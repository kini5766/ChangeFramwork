float4x4 View;
float4x4 Projection;
float4x4 World;

float3 LightDirection = float3(-1, -1, 1);

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
    float3 Color : COLOR0;
};

float3 GetHieghtColor(float y)
{
    float3 color = 0.0f;
    if (y >  15.0f)
        color = float3(1, 0, 0);
    else if (y > 10.0f)
        color = float3(0, 1, 0);
    else if (y > 5.0f)
        color = float3(1, 1, 0);
    else if (y > 0.0f)
        color = float3(0, 0, 1);
    else if (y > -5.0f)
        color = float3(1, 0, 1);
    else if (y > -10.0f)
        color = float3(0, 1, 1);
    else
        color = float3(1, 1, 1);
    
    return color;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Color = GetHieghtColor(output.Position.y);
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    //output.Color = GetHieghtColor(output.Position.y);  // 화면상 위치
    
    output.Normal = mul(input.Normal, (float3x3) World);
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0  // SV_Target[n] <- 랜더 타겟 번호
{
    float3 diffuse = -(1 - input.Color);
    
    float NdotL = dot(normalize(input.Normal), - LightDirection);

	int Posterizing = NdotL * 2;

	NdotL = Posterizing;
	NdotL = NdotL / 2 + 0.5;
    
    // 자신의 색깔끼리는 감쇠 곱하기
    // 스펙큘러 더하기 (안에서는곱하기)
    return float4(diffuse + NdotL, 1);
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