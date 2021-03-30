#pragma once

#include "IEnemySkill.h"
#include "FlowFunction/FlowFunction.h"


struct MagicDesc
{
	class MagicMaker* Maker;
	class IFocus* Player;

	wstring Tag;
	Matrix InitMatrix;

	UINT ClipIdle;
	UINT ClipRun;
	UINT ClipAttack;

	// �����ϱ� ����
	float AttackRangeSq = 25.0f;
	float SkillPower = 5.0f;
	float CoolDown = 1.5f;


	// -- �ǽð����� ��ü���� ������ �ɷ�ġ�� --//
	
	float AttackPower = 0.0f;
	float RunSpeed;
};

class SkillMagic : public IEnemySkill, public IFlowTesk
{
public:
	SkillMagic(const MagicDesc& input, ModelInstance* model);
	~SkillMagic();

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
	MagicDesc desc;
	FutureReturn result;
	float attackTime;  // ���� �õ� �ð� (�� �� ���� ��Ÿ�� ���)
	Transform* transform;
	Animator* animator;
	Transform* bulletTrans;

private:
	class AttackSystem* attacker;
	class RotateSystem* rotator;
	class PointMoveSystem* mover;
	struct MagicBulletInput* bulletInput;

private:
	enum class AttackState {
		None,  // �ƹ��͵� ����
		LookAt,  // �÷��̾� �Ĵٺ�
		BeginAttack,  // ���� ����
		Attacking,  // ���� ��
		EndAttack,  // ���� �� ���
		CoolDown,  // ���� ���� �� ��Ÿ�� ��� ��
		Cansel  // ���߿� �ߴܵ�
	} state = AttackState::None;
};