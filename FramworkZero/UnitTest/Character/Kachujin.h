#pragma once

class KachujinInstance
{
public:
	KachujinInstance(ModelSkinnedInstance* instance);
	~KachujinInstance();

public:
	void Update();

public:
	ModelSkinnedInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelSkinnedInstance* instance;
	Animator* animator;
};