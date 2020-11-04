#include "Framework.h"
#include "ModelClip.h"

ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
	for (auto d : keyframeMap)
		SafeDelete(d.second);
}

ModelKeyframe * ModelClip::Keyframe(wstring name)
{
	if (keyframeMap.count(name) < 1)
		return nullptr;

	return keyframeMap[name];
}
