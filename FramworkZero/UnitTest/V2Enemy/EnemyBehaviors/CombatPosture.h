#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// ���󰡱� : �߰ݹ��� ��
// �ֺ��ɵ��� : ��ų���� �ƴ� * �߰ݹ��� ��
// ��ų��� : ��ų���� ����
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