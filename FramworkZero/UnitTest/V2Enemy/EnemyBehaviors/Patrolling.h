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
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	PatrollingDesc desc;
	DelayReader* reader;

	class LookAround* lookAr;
	vector<class Patrol*> pats;

	const DelayReturn* result;

	DelayReturn bindedReset;
};
