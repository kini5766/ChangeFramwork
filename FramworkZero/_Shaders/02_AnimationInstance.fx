#include "02_SetAnimation.fx"

cbuffer CB_BlendingFrame
{
	BlendDesc BlendingFrames[MODEL_INSTANCE_MAX_COUNT];
};

RWTexture2DArray<float4> Output;

// x : boneIndex
// y : instanceId
[numthreads(1, MODEL_INSTANCE_MAX_COUNT, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	Transform transform;

	Transform curr, next;
	int clip;
	float time;

	clip = BlendingFrames[id.y].Clip[0].Clip;
	time = BlendingFrames[id.y].Clip[0].Time;
	GetAnimWorld(curr, id.x, clip, time);

	float alpha = BlendingFrames[id.y].Alpha;

	transform = curr;
	if (alpha != 0.0f)
	{
		clip = BlendingFrames[id.y].Clip[1].Clip;
		time = BlendingFrames[id.y].Clip[1].Time;
		GetAnimWorld(next, id.x, clip, time);


		if (curr.Scale.x == 0.0f)
		{
			// ¿¡´×º» ¾øÀ½
			transform = curr;
		}
		else if (next.Scale.x != 0.0f)
		{
			Lerp(transform, curr, next, alpha);
		}


	}
	else
	{
		transform = curr;
	}

	matrix world = Combine(transform);
	Output[int3(id.x * 4 + 0, id.y, 0)] = world._11_12_13_14;
	Output[int3(id.x * 4 + 1, id.y, 0)] = world._21_22_23_24;
	Output[int3(id.x * 4 + 2, id.y, 0)] = world._31_32_33_34;
	Output[int3(id.x * 4 + 3, id.y, 0)] = world._41_42_43_44;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}

