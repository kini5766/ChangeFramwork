#pragma once

#include "EnemyBehaviorPieceDesc.h"


class LookAround : public FlowTesk
{
public:
	LookAround(const LookAroundDesc& desc);
	~LookAround();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	LookAroundDesc desc;
	float runningTime = 0.0f;

	FutureReturn result;
};