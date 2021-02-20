#pragma once

class GasMask
{
public:
	static void BindAnimation(Animator* animator, class AnimationAdapter* model);
};

class GasMaskInstance
{
public:
	GasMaskInstance(ModelInstance* instance);
	~GasMaskInstance();

public:
	void Update();

public:
	ModelInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelInstance* instance;
	Animator* animator;
};