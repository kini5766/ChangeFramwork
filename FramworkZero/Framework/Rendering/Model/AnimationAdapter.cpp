#include "Framework.h"
#include "AnimationAdapter.h"

#include "Rendering/Model/ModelAnimationDesc.h"

AnimationAdapter::AnimationAdapter(const UINT clipCount, const ModelClipData*const* clips, BlendDesc* blendingFrames)
	: clipCount(clipCount), clips(clips)
	, blendingFrames(blendingFrames)
{
}

AnimationAdapter::~AnimationAdapter()
{
}

void AnimationAdapter::Update()
{
	funcGetAnimDesc(blendingFrames);
}

void AnimationAdapter::SetAnimator(function<void(struct BlendDesc*out)> func)
{
	funcGetAnimDesc = func;
}
