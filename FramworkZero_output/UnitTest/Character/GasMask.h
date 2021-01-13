#pragma once

class GasMask
{
public:
	static void BindAnimation(Animator* animator, class ModelAnimation* model);
};

class GasMaskInstance
{
public:
	GasMaskInstance(ModelSkinnedInstance* instance);
	~GasMaskInstance();

public:
	void Update();

public:
	ModelSkinnedInstance* GetInstance() { return instance; }
	Animator* GetAnimator() { return animator; }

private:
	ModelSkinnedInstance* instance;
	Animator* animator;
};