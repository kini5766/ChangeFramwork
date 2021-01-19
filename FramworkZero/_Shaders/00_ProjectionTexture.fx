#define PROJECTION_TEXTURE_MAX_COUNT 1


// --
// ProjectionTexture
// --

Texture2DArray ProjectionMaps;

struct ProjectionTextureDesc
{
	float4 Color;
	matrix View;
	matrix Projection;
};

cbuffer CB_ProjectionTexture
{
	uint ProjectionTextureCount;
	float3 CB_ProjectionTexture_Padding;

	ProjectionTextureDesc ProjectionTexture[PROJECTION_TEXTURE_MAX_COUNT];
};


// --
// VS_ProjectionTexture
// --

void VSSet_ProjectionTexture(inout float4 wvp[PROJECTION_TEXTURE_MAX_COUNT], float3 wPosition)
{
	for (uint i = 0; i < ProjectionTextureCount; i++)
	{
		wvp[i] = float4(wPosition, 1.0f);
		wvp[i] = mul(wvp[i], ProjectionTexture[i].View);
		wvp[i] = mul(wvp[i], ProjectionTexture[i].Projection);
	}
}


// --
// PS_ProjectionTexture
// --

void PSSet_ProjectionTexture(in float4 wvp[PROJECTION_TEXTURE_MAX_COUNT], inout float4 color)
{
	// ndc -> uv
	float3 uvw = 0;

	for (uint i = 0; i < ProjectionTextureCount; i++)
	{
		uvw.x = wvp[i].x / wvp[i].w * 0.5f + 0.5f;
		uvw.y = wvp[i].y / wvp[i].w * -0.5f + 0.5f;
		uvw.z = wvp[i].z / wvp[i].w;

		[flatten]
		if (saturate(uvw.x) == uvw.x &&
			saturate(uvw.y) == uvw.y &&
			saturate(uvw.z) == uvw.z)
		{
			float4 pc = ProjectionMaps.Sample(PointSampler, float3(uvw.xy, i));
			pc.rgb += ProjectionTexture[i].Color.rgb;
			color = float4(lerp(color.rgb, pc.rgb, pc.a), color.a);
		}
	}
}
