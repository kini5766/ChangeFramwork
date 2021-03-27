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
	void Hold() override;

private:
	// 행동 캐이스 업데이트
	int UpdateCase();
	FuncCase funcCase;

	// 종료
	void EndCase();
	FutureAction funcEndCase;

	// 이동할 위치 정하기
	void UpdatePositioning();
	FutureAction funcPositioning;

private:
	StrafeAroundDesc desc;
	FutureReturn result;
	FlowSwitching* switching;

	FlowAction* playIdle;
	FlowAction* playRun;

	class Waiter* waiter;
	class Follower* follower;
	class PointMover* moveto;

	FlowRoutine* wait;
	FlowRoutine* move;
	FlowAction* positioning;

	FlowRoutine* followRoutine;
	FlowRoutine* prowlRoutine;

private:
	bool curr = false;

	// 적 놓치면 호출
	function<void()> funcOutRange;
public:
	void SetFuncOutRange(const function<void()>& value) { funcOutRange = value; }
};
