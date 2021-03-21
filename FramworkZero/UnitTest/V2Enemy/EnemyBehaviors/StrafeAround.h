#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// 추격 범위 안 : 주변 맴돌기
// 추격 범위 밖 : 따라가기
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
