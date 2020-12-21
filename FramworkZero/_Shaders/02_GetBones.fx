#define MODEL_INSTANCE_MAX_COUNT 64

cbuffer CB_World
{
	matrix World;
};

struct BoneDesc
{
	int Parent;
	matrix InvBone;
};


Texture2DArray<float4> InputLocalBones;
StructuredBuffer<BoneDesc> InputboneDesc;

RWTexture2DArray<float4> OutputBone;
RWTexture2DArray<float4> OutputSkinned;

void GetWorld(out matrix world, uint3 id)
{
	float4 c0, c1, c2, c3;
	c0 = InputLocalBones[int3(id.x * 4 + 0, id.y, 0)];
	c1 = InputLocalBones[int3(id.x * 4 + 1, id.y, 0)];
	c2 = InputLocalBones[int3(id.x * 4 + 2, id.y, 0)];
	c3 = InputLocalBones[int3(id.x * 4 + 3, id.y, 0)];

	world = matrix(c0, c1, c2, c3);
	int parent = InputboneDesc[id.x].Parent;
	while (parent != -1)
	{
		c0 = InputLocalBones[int3(parent * 4 + 0, id.y, 0)];
		c1 = InputLocalBones[int3(parent * 4 + 1, id.y, 0)];
		c2 = InputLocalBones[int3(parent * 4 + 2, id.y, 0)];
		c3 = InputLocalBones[int3(parent * 4 + 3, id.y, 0)];
		world = mul(world, matrix(c0, c1, c2, c3));

		parent = InputboneDesc[parent].Parent;
	}

	world = mul(world, World);
}

// x : boneIndex
// y : instanceId
[numthreads(1, MODEL_INSTANCE_MAX_COUNT, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	matrix world;
	GetWorld(world, id);
	OutputBone[int3(id.x * 4 + 0, id.y, 0)] = world._11_12_13_14;
	OutputBone[int3(id.x * 4 + 1, id.y, 0)] = world._21_22_23_24;
	OutputBone[int3(id.x * 4 + 2, id.y, 0)] = world._31_32_33_34;
	OutputBone[int3(id.x * 4 + 3, id.y, 0)] = world._41_42_43_44;

	world = mul(InputboneDesc[id.x].InvBone, world);

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
