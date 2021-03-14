#pragma once

#include "EnemyBehaviorDesc.h"


class Patrolling : public IDelayFunction
{
public:
	Patrolling(const PatrollingDesc& desc);
	~Patrolling();

private:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;


private:
	void Reset();
	ReturnAction funcReset;

private:
	PatrollingDesc desc;
	DelayReader* reader;

	class LookAround* lookAr;
	vector<class Patrol*> pats;

	DelayReturn result;
};
