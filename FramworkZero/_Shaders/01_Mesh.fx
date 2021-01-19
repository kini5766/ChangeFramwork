#include "00_Global.fx"
#include "00_PixelOutput.fx"
#include "00_VertexInput.fx"



// >-- None --< //

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



// >-- Shadow + ProjectionTexture --< //

// --
// VS_Shadow (1pass Depth)
// --

DepthOutput VS_Mesh_Inst_Depth(VertexMesh_Inst input)
{
	DepthOutput output = (DepthOutput)0;

	World = input.Transform;

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}
DepthOutput VS_Mesh_Depth(VertexMesh input)
{
	DepthOutput output = (DepthOutput)0;

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}


// --
// VS_Shadow (2pass Shadow)
// --

MeshOutput_Shadow VS_Mesh_Inst_Shadow(VertexMesh_Inst input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow)0;

	World = input.Transform;

	// input -> output
	VS_GENERATE

	VSSet_Shadow(output.sPosition, output.wPosition);


	return output;
}
MeshOutput_Shadow VS_Mesh_Shadow(VertexMesh input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow)0;

	// input -> output
	VS_GENERATE

	VSSet_Shadow(output.sPosition, output.wPosition);


	return output;
}


// --
// Pass
// --

technique11 T0
{
	// None
	P_VP(P0, VS_Mesh, PS_MeshOutput)
	P_VP(P1, VS_Mesh_Inst, PS_MeshOutput)

	// ProjectionTexture
	P_VP(P2, VS_Mesh, PS_MeshOutput_ProjT)
	P_VP(P3, VS_Mesh_Inst, PS_MeshOutput_ProjT)

	// Shadow
	P_RS_VP(P4, FrontCounterClockwise_True, VS_Mesh_Inst_Depth, PS_Shadow_Depth)
	P_VP(P5, VS_Mesh_Inst_Shadow, PS_MeshOutput_Shadow)
	P_RS_VP(P6, FrontCounterClockwise_True, VS_Mesh_Depth, PS_Shadow_Depth)
	P_VP(P7, VS_Mesh_Shadow, PS_MeshOutput_Shadow)
}
