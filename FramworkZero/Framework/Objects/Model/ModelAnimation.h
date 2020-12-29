#pragma once

#include "Objects/Model/ModelAnimationDesc.h"


// ModelAnimation -> 각각 모델클래스 -> Animator
class ModelAnimation
{
public:
	ModelAnimation(const ModelData* clips, BlendDesc* blendingFrames);
	~ModelAnimation();

public:
	void Update();

public:
	UINT GetClipCount() const { return clips->ClipCount(); }
	const ModelClipData* GetClipData(UINT index) { return clips->ClipByIndex(index); }
	void SetAnimator(function<void(BlendDesc*out)> func) { funcGetAnimDesc = func; }

private:
	const ModelData* clips;
	BlendDesc* blendingFrames;
	function<void(BlendDesc*out)> funcGetAnimDesc;
};
