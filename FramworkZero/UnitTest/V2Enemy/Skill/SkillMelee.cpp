#include "stdafx.h"
#include "SkillMelee.h"

#include "Component/AttackSystem.h"


SkillMelee::SkillMelee(const MeleeDesc& desc, Transform* parent)
	: endTime(desc.EndTime)
	, readyTime(desc.ReadyTime)
	, delayTime(desc.DelayTime)
{
	system = new AttackSystem();
	system->GetTransform()->SetParent(parent);
	system->GetTransform()->LocalWorld(desc.InitMatrix);
	system->SetTag(desc.Tag);
	system->SetAttack(desc.Attack);
}

SkillMelee::~SkillMelee()
{
	SafeDelete(system);
}

bool SkillMelee::IsValid()
{
	//if (state == AttackState::None ||
	//	state == AttackState::Stop)
	//	return true;

	return false;
}

void SkillMelee::Call(const FutureAction* future)
{
	runningTime = 0.0f;
	state = AttackState::Ready;
	result.SetAction(future);
}

void SkillMelee::Update()
{
	if (system->IsAttacking())
		Debug::Box->RenderBox(system->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	switch (state)
	{
	case AttackState::None:
		if (runningTime == 0.0f)
		{
			// 공격 시작
			state = AttackState::Ready;
		}
		else
		{
			result.OnAction();
			return;
		}
		break;

	case AttackState::Ready:
		if (runningTime >= readyTime)
		{
			system->StartAttack();
			state = AttackState::Attack;
		}
		break;

	case AttackState::Attack:
		if (runningTime >= endTime)
		{
			system->EndAttack();
			state = AttackState::Delay;
		}
		break;

	case AttackState::Delay:
		if (runningTime >= delayTime)
		{
			state = AttackState::None;
			result.OnAction();
			return;
		}
		break;

	case AttackState::Stop:  // 외부로 인한 종료
		if (runningTime == 0.0f)
		{
			state = AttackState::Ready;
		}
		else
		{
			state = AttackState::None;
			result.OnAction();
			return;
		}
		break;
	}

	runningTime += Time::Delta();
}

void SkillMelee::Cancel()
{
	state = AttackState::Stop;
	result.Clear();
}
