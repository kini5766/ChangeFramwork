#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


class Patrolling : public IFlowTesk
{
public:
	Patrolling(const PatrollingDesc& input);
	~Patrolling();

private:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;


private:
	void Reset();
	FutureAction funcReset;

private:
	PatrollingDesc desc;
	FutureReturn result;

	FlowReader* reader;
	vector<class IFlowTesk*> tesks;

	class Waiter* waiter;
	vector<class PointMover*> pats;

};
