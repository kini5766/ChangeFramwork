#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// ���󰡱� : �߰ݹ��� ��
// �ֺ��ɵ��� : ��ų���� �ƴ� * �߰ݹ��� ��
// ��ų��� : ��ų���� ����
class CombatPosture : public IFlowTesk
{
public:
	CombatPosture(const CombatPostureDesc& input);
	~CombatPosture();

private:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Hold() override;

private:
	void Reset();
	FutureAction funcReset;

private:
	CombatPostureDesc desc;
	FutureReturn result;

	FlowReader* reader;
	vector<IFlowTesk*> tesks;
	class StrafeAround* strafeAround;
};