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
	Judgment UpdateLoop();
	FutureAction PlayWalk();
	FutureAction PlayLookAround();
	Judgment IsAround();

private:
	PatrollingDesc desc;
	FlowLoop* loop;

	vector<FlowRoutine*> patrolls;
	vector<class PointMover*> moveToPoints;
	FlowAction* playWalk;

	FlowRoutine* lookAround;
	FlowAction* playLookAround;
	class Waiter* waiter;
};
