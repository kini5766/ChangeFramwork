#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// 따라가기 : 추격범위 밖
// 주변맴돌기 : 스킬최적 아님 * 추격범위 안
// 스킬사용 : 스킬최적 상태
class CombatPosture : public FlowTesk
{
public:
	CombatPosture(const CombatPostureDesc& input);
	~CombatPosture();

private:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	void Reset();
	FutureAction funcReset;

private:
	CombatPostureDesc desc;
	FutureReturn result;

	FlowReader* reader;
	vector<class FlowTesk*> tesks;
	class StrafeAround* strafeAround;
};