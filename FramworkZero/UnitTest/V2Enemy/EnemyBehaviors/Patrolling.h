#pragma once

#include "EnemyBehaviorDesc.h"

class Patrolling : public IDelayFunction
{
public:
	Patrolling(const PatrollingDesc& desc);
	~Patrolling();

private:
	void Reset();

private:
	// IDelayFunction��(��) ���� ��ӵ�
	virtual void Call(const DelayReturn * result) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	PatrollingDesc desc;
	DelayReader* reader;

	class LookAround* lookAr;
	vector<class Patrol*> pats;

	const DelayReturn* result;

	DelayReturn bindedReset;
};
