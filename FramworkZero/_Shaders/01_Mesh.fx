#include "00_Global.fx"
#include "00_Light.fx"
#include "00_VertexInput.fx"


// --
// VS
// --

MeshOutput VS_Mesh_Inst(VertexMesh_Inst input)
{
	MeshOutput output;

	World = input.Transform;

	// input -> output
	VS_GENERATE

	return output;
}

MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;

	// input -> output
	VS_GENERATE

	return output;
}


// --
// PS
// --

float4 PS(MeshOutput input) : SV_Target0
{
	return Lighting_MeshOutput(input);
}


// --
// Pass
// --

technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
	P_VP(P1, VS_Mesh_Inst, PS)
}