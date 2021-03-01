#pragma once


class ModelClone
{
	virtual Animator* GetAnimator() = 0;
	virtual Transform* GetTransform() = 0;
};

class ModelReplicator
{
	virtual ModelClone* AddClone() = 0;
};
