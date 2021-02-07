#include "00_Global.fx"
#include "00_Light.fx"

struct VertexInput
{
    float4 Position : Position0;
	float2 Uv : Uv0;
};

struct VertexOutput
{
    float4 Position : SV_Position0;
	float2 Uv : Uv0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

	output.Uv = input.Uv;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
    return DiffuseMap.Sample(LinearSampler, input.Uv);
}

technique11 T0
{
	P_DSS_VP(P0, DepthEnable_False, VS, PS)
}
