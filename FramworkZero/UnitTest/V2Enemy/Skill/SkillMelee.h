#pragma once

#include "IEnemySkill.h"
#include "FlowFunction/FlowFunction.h"


struct MeleeDesc
{
	wstring Tag;
	float EndTime = 1.05f;
	float DelayTime = 2.0f;
	float ReadyTime = 0.5f;
	float Attack = 8.0f;
	Matrix InitMatrix;
};

class SkillMelee : public IEnemySkill, public IFlowTesk
{
public:
	SkillMelee(const MeleeDesc& desc, Transform* parent);
	~SkillMelee();

private:
	// IEnemySkill��(��) ���� ��ӵ�
	bool IsValid() override;
	IFlowTesk* GetTesk() override { return this; }

private:
	// IFlowTesk��(��) ���� ��ӵ�
	void Call(const FutureAction* future) override;
	void Update() override;
	void Hold() override;

private:
	float endTime;
	float readyTime;
	float delayTime;
	float runningTime = 0.0f;

	FutureReturn result;

private:
	class AttackSystem* system;
	enum class AttackState {
		None, Ready, Attack, Delay, Stop
	} state = AttackState::None;
};