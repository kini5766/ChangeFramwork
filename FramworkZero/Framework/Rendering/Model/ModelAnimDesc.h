#pragma once

#include "Framework.h"

struct ModelAnimDesc
{
	UINT BoneCount;
	const ModelBoneData*const* Bones;

	UINT ClipCount;
	const ModelClipData*const* Clips;

public:
	ModelAnimDesc() {}
	ModelAnimDesc(ModelData* data)
	{
		BoneCount = data->BoneCount();
		Bones = data->Bones();
		ClipCount = data->ClipCount();
		Clips = data->Clips();
	}
};