#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "01_Billboard.fx"

float4 PS(MeshOutput input) : SV_Target0
{
	float4 c = PS_AllLight(input);
	float4 cmin = PS_MinLight(input);
	return max(c, cmin);
}


technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
	P_VP(P1, VS_Model, PS)
	P_RS_BS_VGP(P2, CullMode_None, AlphaBlend_AlphaToCoverageEnable, VS_Billboard, GS_Billboard, PS_Billboard)
	P_RS_DSS_VP(P3, FrontCounterClockwise_True, DepthEnable_False, VS_Mesh, PS_Sky)
}