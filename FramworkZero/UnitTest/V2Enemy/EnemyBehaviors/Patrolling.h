#pragma once

#include "EnemyBehaviorDesc.h"


class Patrolling : public FlowTesk
{
public:
	Patrolling(const PatrollingDesc& desc);
	~Patrolling();

private:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const FutureAction * action);
	void Update();
	void Cancel();


private:
	void Reset();
	FutureAction funcReset;

private:
	PatrollingDesc desc;
	FlowPlayer* reader;

	class LookAround* lookAr;
	vector<class MovingToPoint*> pats;

	FutureReturn result;
};
