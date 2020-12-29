#pragma once

#include "Objects/Model/ModelAnimationDesc.h"


// ModelAnimation -> 각각 모델클래스 -> Animator
class ModelAnimation
{
public:
	ModelAnimation(const ModelData* clips, KeyframeDesc* frameDesc);
	~ModelAnimation();

public:
	void Update();

public:
	UINT GetClipCount() const { return clips->ClipCount(); }
	const ModelClipData* GetClipData(UINT index) { return clips->ClipByIndex(index); }
	void SetAnimator(function<void(KeyframeDesc*out)> func) { funcGetAnimDesc = func; }

private:
	const ModelData* clips;
	KeyframeDesc* frameDesc;
	function<void(KeyframeDesc*out)> funcGetAnimDesc;
};
