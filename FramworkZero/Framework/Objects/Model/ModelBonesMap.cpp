#include "Framework.h"
#include "ModelBonesMap.h"

ModelBonesMap::ModelBonesMap(ModelData * data)
{
	UINT boneCount = data->BoneCount();
	boneDesc = new BoneDesc[boneCount];

	Transform* transform = new Transform[boneCount];

	for (UINT i = 0; i < boneCount; i++)
	{
		ModelBoneData* bone = data->BoneByIndex(i);
		boneDesc[i].Parent = bone->ParentIndex;

		// 로컬 월드 구하기 -> (DefaultBone)
		if (bone->ParentIndex != -1)
			transform[i].SetParent(transform + bone->ParentIndex);
		transform[i].LossyWorld(bone->Transform);
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
