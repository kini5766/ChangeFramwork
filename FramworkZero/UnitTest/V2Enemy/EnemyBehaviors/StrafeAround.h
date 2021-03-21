#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// �߰� ���� �� : �ֺ� �ɵ���
// �߰� ���� �� : ���󰡱�
class StrafeAround : public FlowTesk
{
public:
	StrafeAround(const StrafeAroundDesc& input);
	~StrafeAround();


private:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	void Reset();
	FutureAction funcReset;


private:
	StrafeAroundDesc desc;
	FutureReturn result;

	FlowReader* reader;

	class Waiter* waiter;
	class Follower* follower;
	class PointMover* arounder;

private:
	bool curr = false;
};
