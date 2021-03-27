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
	Judgment IsAround();

private:
	PatrollingDesc desc;

	// -- Play Animation -- //
	FlowAction* playWalk;
	FlowAction* playLookAround;

	// -- Look Around -- //
	class Waiter* waiter;

	// -- Patrolls -- //
	vector<class PointMover*> moveToPoints;

	// -- Main Routine -- //
	FlowLoop* loop;
	FlowRoutine* lookAround;
	vector<FlowRoutine*> patrolls;
};
