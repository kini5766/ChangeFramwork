#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 albedo = c.rgb;

	float4 result;
	// Lambert
	float3 diffuse = albedo;
	{
		float3 N = normalize(input.Normal);
		float3 L = normalize(GlobalLight.Direction);
		float NdotL = dot(N, -L);

		float theta = acos(NdotL) * 0.31830988618f;
		theta = 1.0f - theta;

		float base = theta * 0.0625f;  // ´Þºû
		float overflow = saturate(theta - 0.5f) * 2.25f;  // ÅÂ¾çºû

		theta = saturate(overflow);

		diffuse = albedo * theta;
		diffuse = clamp(diffuse, albedo * base, 1.0f);
	}
	result.rgb = diffuse;
	result.rgb += input.Color;

	result.a = c.a;

	return result;
}


technique11 T0
{
	P_VP(P0, VS_Mesh, PS)
	P_VP(P1, VS_Model, PS)
}