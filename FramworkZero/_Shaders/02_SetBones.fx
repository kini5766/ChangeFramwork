#define MODEL_INSTANCE_MAX_COUNT 64

struct BoneDesc
{
	int Parent;
	matrix InvBone;
	matrix DefaultBone;
};

Texture2DArray<float4> InputLocalBones;
StructuredBuffer<BoneDesc> InputboneDesc;

void GetWorldLocal(out matrix world, uint boneIndex, uint instance)
{
	float4 c0, c1, c2, c3;
	c0 = InputLocalBones[int3(boneIndex * 4 + 0, instance, 0)];
	c1 = InputLocalBones[int3(boneIndex * 4 + 1, instance, 0)];
	c2 = InputLocalBones[int3(boneIndex * 4 + 2, instance, 0)];
	c3 = InputLocalBones[int3(boneIndex * 4 + 3, instance, 0)];

	world = matrix(c0, c1, c2, c3);

	[flatten]
	if (world._44 == 0.0f)
	{
		// 에닝본 없음
		world = InputboneDesc[boneIndex].DefaultBone;
	}
}

void GetWorld(out matrix world, uint boneIndex, uint instance)
{
	GetWorldLocal(world, boneIndex, instance);

	int f = 0;
	int parent = InputboneDesc[boneIndex].Parent;
	while (parent != -1)
	{
		[flatten]
		if (++f > 100) break;  // 오류 - 잘못 된 값 감지

		matrix parentWorld;
		GetWorldLocal(parentWorld, parent, instance);
		world = mul(world, parentWorld);

		parent = InputboneDesc[parent].Parent;
	}
}