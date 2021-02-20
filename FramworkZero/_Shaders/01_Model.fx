#include "00_Global.fx"
#include "00_PixelOutput.fx"
#include "00_VertexInput.fx"


// >-- None --< //

// --
// VS
// --

MeshOutput VS_Model_Inst(VertexModel_Inst input)
{
	MeshOutput output;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_GENERATE

	return output;
}


// >-- Shadow --< //

// --
// VS_Shadow
// --
// 1pass Depth
DepthOutput VS_Model_Inst_Depth(VertexModel_Inst input)
{
	DepthOutput output = (DepthOutput)0;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}
// 2pass Shadow
MeshOutput_Shadow VS_Model_Inst_Shadow(VertexModel_Inst input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow) 0;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_GENERATE

	VSSet_Shadow(output.sPosition, output.wPosition);

	return output;
}


// --
// VS EnvCube PreRender
// --

MeshOutput_PreEnvCube VS_PreEnvCube_Model_Inst(VertexModel_Inst input)
{
	MeshOutput_PreEnvCube output = (MeshOutput_PreEnvCube)0;
	SetModelWorld_All_Inst(World, input);
	// input -> output
	VS_PREENVCUBE_GENERATE;
	VSSet_Shadow(output.sPosition, output.wPosition);

	return output;
}


technique11 T0
{
	// None
	P_VP(P0, VS_Model_Inst, PS_MeshOutput)

	// Shadow
	P_RS_VP(P1, FrontCounterClockwise_True, VS_Model_Inst_Depth, PS_Shadow_Depth)
	P_VP(P2, VS_Model_Inst_Shadow, PS_MeshOutput_Shadow)

	// EnvCube PreRender
	P_VGP(P3, VS_PreEnvCube_Model_Inst, GS_PreEnvCube_MeshOutput, PS_PreEnvCube_MeshOutput)

	// EnvCube Render
	P_BS_VP(P4, AlphaBlend, VS_Model_Inst_Shadow, PS_MeshOutput_Shadow_EnvCube)
}