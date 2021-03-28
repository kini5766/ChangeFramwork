#include "stdafx.h"
#include "SkillMelee.h"

#include "Rendering/Camera/Main/IFocus.h"
#include "Component/AttackSystem.h"
#include "Component/PointMoveSystem.h"
#include "Component/RotateSystem.h"
#include "V2Enemy/PerceptionSystem.h"


SkillMelee::SkillMelee(const MeleeDesc& input, ModelInstance* model)
	: desc(input)
	, attackTime(-desc.CoolDown)
	, transform(model->GetTransform())
	, animator(model->GetAnimator())
	, funcEndMotion(bind(&SkillMelee::EndMotion, this))
{
	PerceptionInput perceptMaker;
	perceptMaker.FuncGetMine = [=]() {
		Vector3 out;
		transform->Position(&out);
		return out;
	};
	perceptMaker.FuncGetFocus = [=]() {
		Vector3 out;
		desc.Player->Focus(&out);
		return out;
	};
	perceptMaker.LoseRangeSq = desc.AttackRangeSq;
	perceptMaker.SightRangeSq = desc.AttackRangeSq;


	mover = new PointMoveSystem(transform);
	rotator = new RotateSystem();
	attacker = new AttackSystem();
	perceptor = new PerceptionSystem(perceptMaker);


	animator->AddFuncChange(desc.ClipAttack, funcEndMotion);

	mover->SetMoveSpeeder(&desc.RunSpeed);

	attacker->GetTransform()->SetParent(transform);
	attacker->GetTransform()->LocalWorld(desc.InitMatrix);
	attacker->SetTag(desc.Tag);
	attacker->SetAttack(desc.AttackPower * desc.SkillFactor + desc.SkillPower);
}

SkillMelee::~SkillMelee()
{
	SafeDelete(perceptor);
	SafeDelete(attacker);
	SafeDelete(rotator);
	SafeDelete(mover);
}

bool SkillMelee::IsValid()
{
	if (state == AttackState::CoolDown)
	{
		float runningTime = Time::Get()->Running() - attackTime;
		if (runningTime >= desc.CoolDown)
		{
			state = AttackState::None;
		}
	}

	if (state == AttackState::None)
		return true;

	return false;
}

void SkillMelee::Call(const FutureAction* future)
{
	if (state == AttackState::Cansel)
	{
		// 공격이 취소 되었을 경우
		if (attackTime > 0)
		{
			state = AttackState::CoolDown;
		}
		else
		{
			state = AttackState::None;
		}

		(*future)();
		return;
	}

	perceptor->Update();
	if (perceptor->IsPerceived())
	{
		state = AttackState::LookAt;
	}
	else
	{
		state = AttackState::Follow;
	}

	attackTime = -desc.CoolDown;
	result.SetAction(future);
}

void SkillMelee::Update()
{
	if (attacker->IsAttacking())
		Debug::Box->RenderBox(attacker->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	switch (state)
	{
	case AttackState::Follow:
	{
		animator->PlayUpdate(desc.ClipRun);

		Vector3 focus;
		desc.Player->Focus(&focus);
		mover->SetPoint(focus);
		mover->GoToPoint();

		perceptor->Update();
		if (perceptor->IsPerceived())
		{
			state = AttackState::LookAt;
		}
		break;
	}
	case AttackState::LookAt:
	{
		animator->PlayUpdate(desc.ClipRun);

		// 위치
		Vector3 position;
		transform->Position(&position);
		Vector3 point;
		desc.Player->Focus(&point);

		// 거리
		Vector3 destance = point - position;
		destance.y = 0.0f;

		// 방향
		Vector3 direction;
		D3DXVec3Normalize(&direction, &destance);

		// 회전
		float speedDelta = desc.RunSpeed * Time::Delta();
		float rad;
		Quaternion q;
		transform->Rotation(&q);
		rotator->SetDirection(direction);
		q = rotator->GetRotation(q, -transform->Forward(), speedDelta, &rad);
		transform->Rotation(q);

		if (rad <= speedDelta)
		{
			state = AttackState::BeginAttack;
		}
		break;
	}

	case AttackState::BeginAttack:
	{
		state = AttackState::Attacking;
		attackTime = Time::Get()->Running();

		animator->Play(desc.ClipAttack);
		break;
	}
	case AttackState::Attacking:
	{
		float runningTime = Time::Get()->Running() - attackTime;
		if (runningTime >= desc.EndTime)
		{
			attacker->EndAttack();
		}
		else if (runningTime >= desc.ReadyTime)
		{
			if (attacker->IsAttacking() == false)
				attacker->StartAttack();
		}
		break;
	}
	case AttackState::EndAttack:
	{
		state = AttackState::CoolDown;
		result.OnAction();
		break;
	}
	}
}

void SkillMelee::Hold()
{
	state = AttackState::Cansel;
	attacker->EndAttack();
	result.Clear();
}


void SkillMelee::EndMotion()
{
	if (state == AttackState::Attacking)
	{
		state = AttackState::EndAttack;
	}
}
