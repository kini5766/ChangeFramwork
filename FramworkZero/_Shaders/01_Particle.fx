#include "00_Global.fx"

Texture2D ParticleMap;

struct ParticleDesc
{
	float4 MinColor;
	float4 MaxColor;

	float3 Gravity;
	float EndVelocity;

	float2 StartSize;
	float2 EndSize;

	float2 RotateSpeed;
	float ReadyTime;
	float ReadyRandomTime;

	float CurrentTime;
};

cbuffer CB_Particle
{
	ParticleDesc Particle;
};

struct VertexInput
{
	float4 Position : Position;
	float2 Corner : Uv;
	float3 Velocity : Velocity;
	float4 Random : Random;
	float Time : Time;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float2 Uv : Uv;
};

float4 ComputePosition(float3 position, float3 velocity, float age, float t)
{
	float start = length(velocity);
	float end = start * Particle.EndVelocity;  // lerp -> intergral((누)적분)

	// 등가속도 공식
	float intergral = start * t + (end - start) * t * 0.5f;

	// *Particle.ReadyTime : t -> age로 변환
	position += normalize(velocity) * intergral * Particle.ReadyTime;
	position += Particle.Gravity  * t * age;  // 원본
	//position += Particle.Gravity * intergral;

	return ViewProjection(float4(position, 1));
}

float ComputeSize(float random, float t)
{
	float start = lerp(Particle.StartSize.x, Particle.StartSize.y, random);
	float end = lerp(Particle.EndSize.x, Particle.EndSize.y, random);
	return lerp(start, end, t);
}

// roll 회전
float2x2 ComputeRotation(float random, float t)
{
	float speed = lerp(Particle.RotateSpeed.x, Particle.RotateSpeed.y, t);
	float radian = random * speed;

	float c = cos(radian);
	float s = sin(radian);

	return float2x2(c, -s, s, c);
}

float4 ComputeColor(float random, float t)
{
	float4 color = lerp(Particle.MinColor, Particle.MaxColor, random);
	color.a *= t * (1 - t) * (1 - t) * 6.7f;

	return color;
}

VertexOutput VS(VertexInput input)
{
	VertexOutput output = (VertexOutput) 0;

	float age = Particle.CurrentTime - input.Time;
	age *= input.Random.x * Particle.ReadyRandomTime + 1;  // 0방지
	float t = saturate(age / Particle.ReadyTime);

	output.Uv = (input.Corner + 1.0f) * 0.5f;
	output.Color = ComputeColor(input.Random.w, t);
	output.Position = ComputePosition(input.Position.xyz, input.Velocity, age, t);

	float size = ComputeSize(input.Random.y, t);
	float2x2 rotation = ComputeRotation(input.Random.z, age);
	output.Position.xy += mul(input.Corner, rotation) * size * 0.5f;

	return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
	float4 c = ParticleMap.Sample(LinearSampler, input.Uv);
	c *= input.Color;
	return c;
}

technique11 T0
{
	// Opaque
	P_VP(P0, VS, PS)

	// WriteMask Zero PixleDepthEnable_False
	P_DSS_BS_VP(P2, PixleDepthEnable_False, AdditiveBlend, VS, PS)
	P_DSS_BS_VP(P3, PixleDepthEnable_False, AlphaBlend, VS, PS)

	//// Blend = AlphaToCoverage Disable
	//P_BS_VP(P1, AdditiveBlend, VS, PS)
	//P_BS_VP(P2, AlphaBlend, VS, PS)

	//// Blend = AlphaToCoverage Enable
	//P_BS_VP(P3, AdditiveBlend_AlphaToCoverageEnable, VS, PS)
	//P_BS_VP(P4, AlphaBlend_AlphaToCoverageEnable, VS, PS)

	//	// DepthEnable_False
	//P_DSS_VP(P5, DepthEnable_False, VS, PS)
	//P_DSS_BS_VP(P6, DepthEnable_False, AdditiveBlend, VS, PS)
	//P_DSS_BS_VP(P7, DepthEnable_False, AlphaBlend, VS, PS)
}