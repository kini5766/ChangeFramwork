#pragma once

// ModelAnimation -> ���� ��Ŭ���� -> Animator
class AnimationAdapter
{
public:
	AnimationAdapter(UINT clipCount, const ModelClipData*const* clips, struct BlendDesc* blendingFrames);
	~AnimationAdapter();

public:
	void Update();

public:
	UINT GetClipCount() const { return clipCount; }
	const ModelClipData*const* const GetClips() { return clips; }
	void SetAnimator(function<void(struct BlendDesc*out)> func);

private:
	const UINT clipCount;
	const ModelClipData*const* clips;
	struct BlendDesc* blendingFrames;
	function<void(struct BlendDesc*out)> funcGetAnimDesc;
};
