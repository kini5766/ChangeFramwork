#pragma once

class Kachujin
{
public:
	static void BindAnimation(Animator* animator, class ModelAnimation* model);
};

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