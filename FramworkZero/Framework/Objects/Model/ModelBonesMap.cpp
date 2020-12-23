#include "Framework.h"
#include "ModelBonesMap.h"

ModelBonesMap::ModelBonesMap(ModelData * data)
{
	UINT boneCount = data->BoneCount();
	boneDesc = new BoneDesc[boneCount];
	for (UINT i = 0; i < boneCount; i++)
	{
		ModelBoneData* bone = data->BoneByIndex(i);

		boneDesc[i].Parent = bone->ParentIndex;
		D3DXMatrixInverse(&boneDesc[i].InvBone, nullptr, &bone->Transform);
		boneDesc[i].DefaultBone = bone->Transform;
	}
	computeBoneDescBuffer = new StructuredBuffer(boneDesc, sizeof(BoneDesc), boneCount);
}

ModelBonesMap::~ModelBonesMap()
{
	SafeDelete(computeBoneDescBuffer);
	SafeDeleteArray(boneDesc);
}
