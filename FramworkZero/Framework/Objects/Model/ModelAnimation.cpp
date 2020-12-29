#include "Framework.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(const ModelData* clips, KeyframeDesc* frameDesc)
	: clips(clips), frameDesc(frameDesc)
{
}

ModelAnimation::~ModelAnimation()
{
}

void ModelAnimation::Update()
{
	funcGetAnimDesc(frameDesc);
}
