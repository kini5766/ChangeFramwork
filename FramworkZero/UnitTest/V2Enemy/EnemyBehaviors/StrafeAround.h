#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// 추격 범위 안 : 주변 맴돌기
// 추격 범위 밖 : 따라가기
class StrafeAround : public IFlowTesk
{
public:
	StrafeAround(const StrafeAroundDesc& input);
	~StrafeAround();


private:
	void Call(const FutureAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	void Reset();
	FutureAction funcReset;
	FutureAction PlayIdle();
	FutureAction PlayRun();

private:
	StrafeAroundDesc desc;
	FutureReturn result;

	FlowReader* reader;

	FlowRoutine* wait;
	FlowAction* playIdle;
	class Waiter* waiter;

	FlowAction* playRun;
	FlowRoutine* follow;
	class Follower* follower;
	FlowRoutine* around;
	class PointMover* moveto;

private:
	bool curr = false;
};
