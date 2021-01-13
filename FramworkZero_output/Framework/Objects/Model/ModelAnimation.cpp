#include "Framework.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(const ModelData* clips, BlendDesc* blendingFrames)
	: clips(clips), blendingFrames(blendingFrames)
{
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	funcGetAnimDesc(blendingFrames);
}
