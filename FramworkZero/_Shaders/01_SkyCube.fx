#include "00_Global.fx"
#include "00_VertexInput.fx"
#include "00_EnvCubeMap.fx"

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
// GS PreEnvCube
// --
struct GSOutput_PreEnvCube
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;

	uint TargetIndex : SV_RenderTargetArrayIndex;  // EnvCube
};

[maxvertexcount(18)]
void GS_PreEnvCube_Sky(triangle VertexOutput input[3], inout TriangleStream<GSOutput_PreEnvCube> stream)
{
	int vertex = 0;
	GSOutput_PreEnvCube output;

	[unroll(6)]
	for (int i = 0; i < 6; i++)
	{
		// SV_·»´õÅ¸°Ù [i]
		output.TargetIndex = i;

		[unroll(3)]
		for (vertex = 0; vertex < 3; vertex++)
		{
			float4 wPosition = WorldPosition(float4(input[vertex].oPosition, 1.0f));
			output.Position = mul(wPosition, PreEnvCube.Views[i]);
			output.Position = mul(output.Position, PreEnvCube.Projection);

			output.oPosition = input[vertex].oPosition;
			stream.Append(output);
		}

		stream.RestartStrip();
	}
}

float4 PS_PreEnvCube_Sky(GSOutput_PreEnvCube input) : SV_Target0
{
	return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}


// --
// Pass
// --

technique11 T0
{
	P_RS_DSS_VP(P0, FrontCounterClockwise_True, DepthEnable_False, VS_Sky, PS_Sky)
	P_RS_DSS_VGP(P1, FrontCounterClockwise_True, DepthEnable_False, VS_Sky, GS_PreEnvCube_Sky, PS_PreEnvCube_Sky)
}