#pragma once

#include "EnemyBehaviorPieceDesc.h"


class Follow : public IDelayFunction
{
public:
	Follow(const FollowDesc& desc);
	~Follow();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	FollowDesc desc;

	const DelayReturn* result;
};