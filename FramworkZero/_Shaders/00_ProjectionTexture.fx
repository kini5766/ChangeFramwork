

// --
// ProjectionTexture
// --

Texture2D ProjectionMap;

struct ProjectionTextureDesc
{
	float4 Color;
	matrix View;
	matrix Projection;
};

cbuffer CB_ProjectionTexture
{
	ProjectionTextureDesc ProjectionTexture;
};


// --
// VS_ProjectionTexture
// --

void VSSet_ProjectionTexture(inout float4 wvp, float3 wPosition)
{
	wvp = float4(wPosition, 1.0f);
	wvp = mul(wvp, ProjectionTexture.View);
	wvp = mul(wvp, ProjectionTexture.Projection);
}


// --
// PS_ProjectionTexture
// --

void PSSet_ProjectionTexture(float4 wvp, inout float4 color)
{
	// ndc -> uv
	float3 uvw = 0;

	uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
	uvw.y = wvp.y / wvp.w * -0.5f + 0.5f;
	uvw.z = wvp.z / wvp.w;

	[flatten]
	if (saturate(uvw.x) == uvw.x &&
		saturate(uvw.y) == uvw.y &&
		saturate(uvw.z) == uvw.z)
	{
		float4 pc = ProjectionMap.Sample(PointSampler, uvw.xy);
		pc.rgb += ProjectionTexture.Color.rgb;
		color = float4(lerp(color.rgb, pc.rgb, pc.a), color.a);
	}
}





// > -- ���� �� -- < //


// --
// ProjectionTextures
// --

#define PROJECTION_TEXTURES_MAX_COUNT 32

Texture2DArray ProjectionMaps;

cbuffer CB_ProjectionTextures
{
	uint ProjectionTextureCount;
	float3 CB_ProjectionTextures_Padding;

	ProjectionTextureDesc ProjectionTextures[PROJECTION_TEXTURES_MAX_COUNT];
};

// --
// VS_ProjectionTextures
// --

void VSSet_ProjectionTextures(inout float4 wvp[PROJECTION_TEXTURES_MAX_COUNT], float3 wPosition)
{
	for (uint i = 0; i < ProjectionTextureCount; i++)
	{
		wvp[i] = float4(wPosition, 1.0f);
		wvp[i] = mul(wvp[i], ProjectionTexture.View);
		wvp[i] = mul(wvp[i], ProjectionTexture.Projection);
	}
}

// --
// PS_ProjectionTextures
// --

void PSSet_ProjectionTextures(in float4 wvp[PROJECTION_TEXTURES_MAX_COUNT], inout float4 color)
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
			float4 pc = ProjectionMap.Sample(PointSampler, uvw.xy);
			pc.rgb += ProjectionTexture.Color.rgb;
			color = float4(lerp(color.rgb, pc.rgb, pc.a), color.a);
		}
	}
}
