#pragma once

#include "EnemyBehaviorPieceDesc.h"


class LookAround : public IDelayFunction
{
public:
	LookAround(const LookAroundDesc& desc);
	~LookAround();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	LookAroundDesc desc;
	float runningTime = 0.0f;

	DelayReturn result;
};