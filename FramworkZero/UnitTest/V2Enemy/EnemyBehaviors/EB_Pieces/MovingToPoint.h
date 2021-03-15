#pragma once

#include "EnemyBehaviorPieceDesc.h"

class MovingToPoint : public FlowTesk
{
public:
	MovingToPoint(const MovingToPointDesc& desc);
	~MovingToPoint();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	MovingToPointDesc desc;

	FutureReturn result;
};
