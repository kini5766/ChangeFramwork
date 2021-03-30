#include "stdafx.h"
#include "SkillMagic.h"

#include "Rendering/Camera/Main/IFocus.h"
#include "Component/RotateSystem.h"
#include "V2Enemy/PerceptionSystem.h"
#include "MagicMaker.h"


SkillMagic::SkillMagic(const MagicDesc& input, ModelInstance* model)
	: desc(input)
	, attackTime(-desc.CoolDown)
	, transform(model->GetTransform())
	, animator(model->GetAnimator())
	, funcEndMotion(bind(&SkillMagic::EndMotion, this))
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


	rotator = new RotateSystem();
	bulletInput = new MagicBulletInput();
	bulletTrans = new Transform();

	bulletInput->Attack = desc.SkillPower;
	bulletInput->Focus = desc.Player;
	bulletInput->Tag = desc.Tag;

	animator->AddFuncChange(desc.ClipAttack, funcEndMotion);

	bulletTrans->SetParent(transform);
	bulletTrans->LocalWorld(desc.InitMatrix);
}

SkillMagic::~SkillMagic()
{
	SafeDelete(bulletTrans);
	SafeDelete(bulletInput);
	SafeDelete(rotator);
}

bool SkillMagic::IsValid()
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

void SkillMagic::Call(const FutureAction* future)
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

	state = AttackState::LookAt;

	attackTime = -desc.CoolDown;
	result.SetAction(future);
}

void SkillMagic::Update()
{
	switch (state)
	{
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
		Matrix w;
		bulletTrans->GlobalWorld(&w);
		Vector3 s, t;
		Quaternion r;
		D3DXMatrixDecompose(&s, &r, &t, &w);

		bulletInput->Point = t;
		bulletInput->Rotation = r;
		desc.Maker->AddBullet(*bulletInput);

		attackTime = Time::Get()->Running();
		animator->Play(desc.ClipAttack);
		state = AttackState::Attacking;

		break;
	}
	case AttackState::Attacking:
	{

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

void SkillMagic::Hold()
{
	state = AttackState::Cansel;
	result.Clear();
}


void SkillMagic::EndMotion()
{
	if (state == AttackState::Attacking)
	{
		state = AttackState::EndAttack;
	}
}
