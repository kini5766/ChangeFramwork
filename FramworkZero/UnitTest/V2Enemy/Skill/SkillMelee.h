#pragma once

#include "IEnemySkill.h"
#include "FlowFunction/FlowFunction.h"


struct MeleeDesc
{
	wstring Tag;
	Matrix InitMatrix;

	float ReadyTime = 0.5f;
	float EndTime = 1.05f;
	float CoolDown = 3.0f;
	// �����ϱ� ����
	float AttackRangeSq = 25.0f;
	// ���� ���
	float SkillFactor = 0.0f;
	// �⺻ ���� ������
	float SkillPower = 8.0f;

	class IFocus* Player;
	UINT ClipIdle;
	UINT ClipRun;
	UINT ClipAttack;


	// -- �ǽð����� ��ü���� ������ �ɷ�ġ�� --//
	
	float AttackPower = 0.0f;
	float RunSpeed;
};

class SkillMelee : public IEnemySkill, public IFlowTesk
{
public:
	SkillMelee(const MeleeDesc& input, ModelInstance* model);
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
	void EndMotion();
	function<void(void)> funcEndMotion;

private:
	MeleeDesc desc;
	FutureReturn result;
	float attackTime;  // ���� �õ� �ð� (�� �� ���� ��Ÿ�� ���)
	Transform* transform;
	Animator* animator;

private:
	class AttackSystem* attacker;
	class RotateSystem* rotator;
	class PointMoveSystem* mover;
	class PerceptionSystem* perceptor;

private:
	enum class AttackState {
		None,  // �ƹ��͵� ����
		Follow,  // �÷��̾� ����
		LookAt,  // �÷��̾� �Ĵٺ�
		BeginAttack,  // ���� ����
		Attacking,  // ���� ��
		EndAttack,  // ���� �� ���
		CoolDown,  // ���� ���� �� ��Ÿ�� ��� ��
		Cansel  // ���߿� �ߴܵ�
	} state = AttackState::None;
};