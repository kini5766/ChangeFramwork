#pragma once

#include "EnemyBehaviorPieceDesc.h"


class Follow : public FlowTesk
{
public:
	Follow(const FollowDesc& desc);
	~Follow();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const FutureAction * result);
	void Update();
	void Cancel();

private:
	FollowDesc desc;

	FutureReturn result;
};
