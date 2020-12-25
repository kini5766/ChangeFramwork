#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target0
{
	Texture(Material.Diffuse, DiffuseMap, input.Uv);
	Texture(Material.Specular, SpecularMap, input.Uv);

	MaterialDesc output;
	ComputeMaterial(output, input.Normal, input.wPosition);

	return float4(MaterialToColor(output), 1.0f) + input.Color;
}


technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
	P_VP(P1, VS_Model, PS)
}