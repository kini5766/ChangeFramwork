#include "Framework.h"
#include "ModelBonesMap.h"

#include "ModelAnimDesc.h"

ModelBonesMap::ModelBonesMap(const ModelAnimDesc& anim)
{
	UINT boneCount = anim.BoneCount;
	boneDesc = new BoneDesc[boneCount];

	Transform* transform = new Transform[boneCount];

	for (UINT i = 0; i < boneCount; i++)
	{
		const ModelBoneData* bone = anim.Bones[i];
		boneDesc[i].Parent = bone->ParentIndex;

		// ���� ���� ���ϱ� -> (DefaultBone)
		if (bone->ParentIndex != -1)
			transform[i].SetParent(transform + bone->ParentIndex);
		transform[i].GlobalWorld(bone->Transform);
		transform[i].LocalWorld(&boneDesc[i].DefaultBone);
	}
	computeBoneDescBuffer = new StructuredBuffer(boneDesc, sizeof(BoneDesc), boneCount);

	SafeDeleteArray(transform);
}

ModelBonesMap::~ModelBonesMap()
{
	SafeDelete(computeBoneDescBuffer);
	SafeDeleteArray(boneDesc);
}
