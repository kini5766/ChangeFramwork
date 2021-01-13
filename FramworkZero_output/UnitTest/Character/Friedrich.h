#pragma once

class Friedrich
{
public:
	static void BindAnimation(Animator* animator, class ModelAnimation* model);
};

class FriedrichInstance
{
public:
	FriedrichInstance(ModelSkinnedInstance* instance);
	~FriedrichInstance();

public:
	void Update();

public:
	ModelSkinnedInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelSkinnedInstance* instance;
	Animator* animator;
};