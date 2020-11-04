#include "00_Global.fx"

cbuffer CB_Snow
{
	float4 Color;

	float3 Velocity;  // �������� �ӵ�
	float DrawDistance = 0;  // ������ �Ÿ�

	float3 Origin;  // �������� ����
	float Turblulence = 5;  // ��� �Ÿ�

	float3 Extent;  // �������� ������ ũ��
};

struct VertexInput
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
	float Scale : SCALE0;
	float2 Random : RANDOM0;
};

struct VertexOutput
{
	float4 Position : SV_POSITION0;
	float2 Uv : UV0;
	float Alpha : ALPHA0;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;

	float3 displace = Time * Velocity;

	input.Position.x += cos(Time - input.Random.x) * Turblulence;
	input.Position.z += cos(Time - input.Random.y) * Turblulence;
	input.Position.y = Origin.y + Extent.y - (input.Position.y - displace.y) % Extent.y;

	input.Position.xyz = Origin + (Extent + (input.Position.xyz + displace) % Extent) % Extent - (Extent * 0.5f);

	float4 position = WorldPosition(input.Position);

	float3 up = normalize(-Velocity);
	float3 forward = position.xyz - ViewPosition();
	float3 right = normalize(cross(up, forward));

	position.xyz += (input.Uv.x - 0.5f) * right * input.Scale;
	position.xyz += (1.0f - input.Uv.y - 0.5f) * up * input.Scale;
	position.w = 1.0f;  // 0�� ���� 1�� ��ġ

	output.Position = ViewProjection(position);
	output.Uv = input.Uv;

	float4 view = mul(position, View);
	output.Alpha = saturate(1 - view.z / DrawDistance);
	//output.Alpha = saturate(view.z / DrawDistance);

	return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);

	color.rgb = Color.rgb * input.Alpha * 2.0f;
	color.a = color.a * input.Alpha;

    return color;
}

technique11 T0
{
	P_BS_VP(P0, AlphaBlend_AlphaToCoverage, VS, PS)
}