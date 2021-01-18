#include "00_Global.fx"

#include "00_ProjectionTexture.fx"
#include "00_Shadow.fx"

#include "00_Light.fx"
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


// --
// PS
// --

float4 PS(MeshOutput input) : SV_Target0
{
	return Lighting_MeshOutput(input);
}



// >-- VS_ProjectionTexture --< //

// --
// VS_ProjectionTexture
// --

MeshOutput_Shadow VS_Mesh_Inst_ProjectionTexture(VertexMesh_Inst input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow)0;

	World = input.Transform;

	// input -> output
	VS_GENERATE

	VSSet_ProjectionTexture(output.wvpPosition_Sub, output.wPosition);

	return output;
}
MeshOutput_Shadow VS_Mesh_ProjectionTexture(VertexMesh input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow)0;

	// input -> output
	VS_GENERATE

	VSSet_ProjectionTexture(output.wvpPosition_Sub, output.wPosition);

	return output;
}

// --
// PS_ProjectionTexture
// --

float4 PS_ProjectionTexture(MeshOutput_Shadow input) : SV_Target0
{
	return Lighting_MeshOutput_ProjectionTexture(input);
}



// >-- Shadow + ProjectionTexture --< //

// --
// VS_Shadow (1pass Depth)
// --

MeshDepthOutput VS_Mesh_Inst_Depth(VertexMesh_Inst input)
{
	MeshDepthOutput output = (MeshDepthOutput)0;

	World = input.Transform;

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}
MeshDepthOutput VS_Mesh_Depth(VertexMesh input)
{
	MeshDepthOutput output = (MeshDepthOutput)0;

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

	VSSet_ProjectionTexture(output.wvpPosition_Sub, output.wPosition);
	VSSet_Shadow(output.sPosition, output.wPosition);


	return output;
}
MeshOutput_Shadow VS_Mesh_Shadow(VertexMesh input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow)0;

	// input -> output
	VS_GENERATE

	VSSet_ProjectionTexture(output.wvpPosition_Sub, output.wPosition);
	VSSet_Shadow(output.sPosition, output.wPosition);


	return output;
}

// --
// PS_Shadow
// --
// 2pass Shadow
float4 PS_Shadow(MeshOutput_Shadow input) : SV_Target0
{
	return Lighting_MeshOutput_Shadow(input);
}


// --
// Pass
// --

technique11 T0
{
	// None
	P_VP(P0, VS_Mesh, PS)
	P_VP(P1, VS_Mesh_Inst, PS)

	// ProjectionTexture
	P_VP(P2, VS_Mesh_Inst_ProjectionTexture, PS_ProjectionTexture)
	P_VP(P3, VS_Mesh_ProjectionTexture, PS_ProjectionTexture)

	// Shadow + ProjectionTexture
	P_VP(P4, VS_Mesh_Inst_Depth, PS_Shadow_Depth)
	P_VP(P5, VS_Mesh_Inst_Shadow, PS_Shadow)
	P_VP(P6, VS_Mesh_Depth, PS_Shadow_Depth)
	P_VP(P7, VS_Mesh_Shadow, PS_Shadow)
}
