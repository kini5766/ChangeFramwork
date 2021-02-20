#pragma once

class Paladin
{
public:
	static void BindAnimation(Animator* animator, class AnimationAdapter* model);
};

class PaladinInstance
{
public:
	PaladinInstance(ModelInstance* instance);
	~PaladinInstance();

public:
	void Update();

public:
	ModelInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelInstance* instance;
	Animator* animator;
};