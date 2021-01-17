#include "00_Global.fx"
#include "00_VertexInput.fx"

TextureCube SkyCubeMap;

// --
// struct
// --

// output
struct VertexOutput
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
};


// --
// VS
// --

VertexOutput VS_Sky(VertexMesh input)
{
	VertexOutput output;

	output.oPosition = input.Position.xyz;
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	return output;
}


// --
// SkyCube
// --

float4 PS_Sky(VertexOutput input) : SV_Target0
{
	return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}


// --
// Pass
// --

technique11 T0
{
	P_RS_DSS_VP(P0, FrontCounterClockwise_True, DepthEnable_False, VS_Sky, PS_Sky)
}