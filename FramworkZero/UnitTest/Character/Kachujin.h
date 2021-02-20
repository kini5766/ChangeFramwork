#pragma once

class Kachujin
{
public:
	static void BindAnimation(Animator* animator, class AnimationAdapter* model);
};

class KachujinInstance
{
public:
	KachujinInstance(ModelInstance* instance);
	~KachujinInstance();

public:
	void Update();

public:
	ModelInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelInstance* instance;
	Animator* animator;
};