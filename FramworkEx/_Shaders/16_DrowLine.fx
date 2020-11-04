#include "00_Global.fx"

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};
    
VertexOutput VS(VertexColor input)
{
    VertexOutput output;
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    output.Color = input.Color;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    return input.Color;
}

technique11 T0
{
    P_VP(P0, VS, PS)
}