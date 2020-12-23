#define MODEL_INSTANCE_MAX_COUNT 64

struct BoneDesc
{
	int Parent;
	matrix InvBone;
	matrix DefaultBone;
};

Texture2DArray<float4> InputLocalBones;
StructuredBuffer<BoneDesc> InputboneDesc;

bool GetWorld(out matrix world, uint boneIndex, uint instance)
{
	float4 c0, c1, c2, c3;
	c0 = InputLocalBones[int3(boneIndex * 4 + 0, instance, 0)];
	c1 = InputLocalBones[int3(boneIndex * 4 + 1, instance, 0)];
	c2 = InputLocalBones[int3(boneIndex * 4 + 2, instance, 0)];
	c3 = InputLocalBones[int3(boneIndex * 4 + 3, instance, 0)];

	world = matrix(c0, c1, c2, c3);
	if (world._44 == 0.0f)
	{
		// 에닝본 없음
		return false;
	}

	int f = 0;
	int parent = InputboneDesc[boneIndex].Parent;
	while (parent != -1)
	{
		if (++f > 100) break;

		c0 = InputLocalBones[int3(parent * 4 + 0, instance, 0)];
		c1 = InputLocalBones[int3(parent * 4 + 1, instance, 0)];
		c2 = InputLocalBones[int3(parent * 4 + 2, instance, 0)];
		c3 = InputLocalBones[int3(parent * 4 + 3, instance, 0)];

		// 부모가 애닝본 없으면 자식이 있을 가능성 없음(혹시 오류나면 수정)
		world = mul(world, matrix(c0, c1, c2, c3));

		parent = InputboneDesc[parent].Parent;
	}

	return true;
}
