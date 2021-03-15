#pragma once

#include "EnemyBehaviorPieceDesc.h"

class MovingToPoint : public FlowTesk
{
public:
	MovingToPoint(const MovingToPointDesc& desc);
	~MovingToPoint();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	MovingToPointDesc desc;

	FutureReturn result;
};
