#include "02_SetBones.fx"


RWTexture2DArray<float4> OutputSkinned;

// x : boneIndex
// y : instanceId
[numthreads(1, MODEL_INSTANCE_MAX_COUNT, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	matrix world;
	GetWorld(world, id.x, id.y);

	// 메쉬본에서 애닝본으로 이동 
	world = mul(InputboneDesc[id.x].InvBone, world);
	// world = mul(world, World);
	OutputSkinned[int3(id.x * 4 + 0, id.y, 0)] = world._11_12_13_14;
	OutputSkinned[int3(id.x * 4 + 1, id.y, 0)] = world._21_22_23_24;
	OutputSkinned[int3(id.x * 4 + 2, id.y, 0)] = world._31_32_33_34;
	OutputSkinned[int3(id.x * 4 + 3, id.y, 0)] = world._41_42_43_44;
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
