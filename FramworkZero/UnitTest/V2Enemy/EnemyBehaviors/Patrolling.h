#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


class Patrolling : public FlowTesk
{
public:
	Patrolling(const PatrollingDesc& desc);
	~Patrolling();

private:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();


private:
	void Reset();
	FutureAction funcReset;

private:
	PatrollingDesc desc;
	FlowReader* reader;
	vector<class FlowTesk*> tesks;

	class Waiter* waiter;
	vector<class PointMover*> pats;

	FutureReturn result;
};
