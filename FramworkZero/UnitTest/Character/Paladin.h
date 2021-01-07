#pragma once

class PaladinInstance
{
public:
	PaladinInstance(ModelSkinnedInstance* instance);
	~PaladinInstance();

public:
	void Update();

public:
	ModelSkinnedInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelSkinnedInstance* instance;
	Animator* animator;
};