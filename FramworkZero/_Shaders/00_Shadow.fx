
// --
// VS_DEPTH_GENERATE
// --

// input -> output
#define VS_DEPTH_GENERATE \
output.Position = WorldPosition(input.Position); \
float3 wPosition = output.Position.xyz; \
output.Position = mul(output.Position, Shadow.View); \
output.Position = mul(output.Position, Shadow.Projection); \
\
output.sPosition = output.Position; \
\
output.Culling.x = dot(float4(wPosition, 1), Culling[0]); \
output.Culling.y = dot(float4(wPosition, 1), Culling[1]); \
output.Culling.z = dot(float4(wPosition, 1), Culling[2]); \
output.Culling.w = dot(float4(wPosition, 1), Culling[3]); \
\
output.Clipping = float4(0, 0, 0, 0); \
output.Clipping.x = dot(float4(wPosition, 1), Clipping); \



// --
// Shadow
// --

SamplerComparisonState ShadowSampler;
Texture2D ShadowMap;

struct ShadowDesc
{
	matrix View;
	matrix Projection;

	float2 MapSize;
	float Bias;

	uint Quality;
};

cbuffer CB_Shadow
{
	ShadowDesc Shadow;
};


// --
// PS_Shadow_Depth
// --
struct DepthOutput
{
	float4 Position : SV_POSITION0;
	float4 sPosition : POSITION1;  // 조명 기준

	float4 Culling : SV_CullDistance;
	float4 Clipping : SV_ClipDistance;
};
float4 PS_Shadow_Depth(DepthOutput input) : SV_Target
{
	// 디버깅용
	float depth = input.sPosition.z / input.sPosition.w;

	//return float4(depth, depth, depth, 1);
	return float4(1, 1, 1, 1);
}


// --
// VS_Shadow
// --

void VSSet_Shadow(out float4 sPosition, float3 wPosition)
{
	sPosition = float4(wPosition, 1.0f);
	sPosition = mul(sPosition, Shadow.View);
	sPosition = mul(sPosition, Shadow.Projection);
}


// --
// PS_Shadow
// --
float2 Offsets[9] =
{
	float2(-1, -1), float2(+0, -1), float2(+1, -1),
	float2(-1, -0), float2(+0, -0), float2(+1, -0),
	float2(-1, +1), float2(+0, +1), float2(+1, +1)
};
float4 PSSet_Shadow(float4 sPosition, float4 color)
{
	float4 sPos = sPosition;
	sPos.xyz /= sPos.w;

	[flatten]
	if (sPos.x < -1.0f || sPos.x > 1.0f &&
		sPos.y < -1.0f || sPos.y > 1.0f &&
		sPos.z < 0.0f || sPos.z > 1.0f)
	{
		//return float4(1, 1, 1, 1);
		return color;
	}

	sPos.x = sPos.x * 0.5f + 0.5f;
	sPos.y = sPos.y * -0.5f + 0.5f;

	float depth = 0.0f;
	float z = sPos.z - Shadow.Bias;
	float factor = 0;


	if (Shadow.Quality == 0)
	{
		depth = ShadowMap.Sample(LinearSampler, sPos.xy).r;  // r32 포멧
		factor = (float)(depth >= z);  // 0(거짓) 아니면 1(참)
	}
	else if (Shadow.Quality == 1)
	{
		depth = z;
		// SampleCmp : Direct11, SamplerState cpp로만 정의 가능한 옵션이 있음
		factor = ShadowMap.SampleCmp(ShadowSampler, sPos.xy, depth).r;  // r32 포멧
	}
	else if (Shadow.Quality == 2)
	{
		depth = z;

		float sum = 0;
		float2 uv = 0;
		float2 pixelSize = 1.0f / Shadow.MapSize;

		[unroll(9)]
		for (uint i = 0; i < 9; i++)
		{
			uv = sPos.xy + Offsets[i] * pixelSize;
			float c = ShadowMap.SampleCmp(ShadowSampler, uv, depth).r;
			sum += c;
		}

		factor = sum / 9.0f;
	}

	//factor = saturate(factor + depth);
	//return float4(factor, factor, factor, 1);
	return float4(color.rgb * factor, 1);
}
