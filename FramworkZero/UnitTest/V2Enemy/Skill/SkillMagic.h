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

	// 공격하기 범위
	float AttackRangeSq = 25.0f;
	float SkillPower = 5.0f;
	float CoolDown = 1.5f;


	// -- 실시간으로 본체에서 얻어야할 능력치들 --//
	
	float AttackPower = 0.0f;
	float RunSpeed;
};

class SkillMagic : public IEnemySkill, public IFlowTesk
{
public:
	SkillMagic(const MagicDesc& input, ModelInstance* model);
	~SkillMagic();

private:
	// IEnemySkill을(를) 통해 상속됨
	bool IsValid() override;
	IFlowTesk* GetTesk() override { return this; }

private:
	// IFlowTesk을(를) 통해 상속됨
	void Call(const FutureAction* future) override;
	void Update() override;
	void Hold() override;

private:
	void EndMotion();
	function<void(void)> funcEndMotion;

private:
	MagicDesc desc;
	FutureReturn result;
	float attackTime;  // 공격 시도 시간 (이 때 부터 쿨타임 계산)
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
		None,  // 아무것도 안함
		LookAt,  // 플레이어 쳐다봄
		BeginAttack,  // 공격 시작
		Attacking,  // 공격 중
		EndAttack,  // 공격 뒤 대기
		CoolDown,  // 공격 성공 시 쿨타임 대기 중
		Cansel  // 도중에 중단됨
	} state = AttackState::None;
};