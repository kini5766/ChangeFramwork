#pragma once

class Friedrich
{
public:
	static void BindAnimation(Animator* animator, class AnimationAdapter* model);
};

class FriedrichInstance
{
public:
	FriedrichInstance(ModelInstance* instance);
	~FriedrichInstance();

public:
	void Update();

public:
	ModelInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelInstance* instance;
	Animator* animator;
};