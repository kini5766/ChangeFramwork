#include "02_Animation.fx"

cbuffer CB_Keyframes
{
	KeyframeDesc Keyframes[MODEL_INSTANCE_MAX_COUNT];
};

RWTexture2DArray<float4> Output;

// x : boneIndex
// y : instanceId
[numthreads(1, MODEL_INSTANCE_MAX_COUNT, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	Transform transform = (Transform)0;

	int clip = Keyframes[id.y].Time;
	float time = Keyframes[id.y].Clip;
	GetAnimWorld(transform, id.x, clip, time);

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

