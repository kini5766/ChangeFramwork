#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


class Patrolling : public FlowTesk
{
public:
	Patrolling(const PatrollingDesc& input);
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
	FutureReturn result;

	FlowReader* reader;
	vector<class FlowTesk*> tesks;

	class Waiter* waiter;
	vector<class PointMover*> pats;

};
