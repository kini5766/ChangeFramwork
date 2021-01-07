#include "stdafx.h"
#include "EnemyAttackSystem.h"

#include "Component/AttackSystem.h"

EnemyAttackSystem::EnemyAttackSystem()
{
	system = new AttackSystem();
}

EnemyAttackSystem::~EnemyAttackSystem()
{
	SafeDelete(system);
}

void EnemyAttackSystem::Update()
{
	if (system->IsAttacking())
		Debug::Box->RenderBox(system->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
	
	switch (state)
	{
	case EnemyAttackSystem::AttackState::None:
		break;
	case EnemyAttackSystem::AttackState::Ready:
		runningTime += Time::Delta();
		if (runningTime >= readyTime)
		{
			system->StartAttack();
			state = AttackState::Attack;
		}
		break;
	case EnemyAttackSystem::AttackState::Attack:
		runningTime += Time::Delta();
		if (runningTime >= endTime)
		{
			system->EndAttack();
			state = AttackState::Delay;
		}
		break;
	case EnemyAttackSystem::AttackState::Delay:
		runningTime += Time::Delta();
		if (runningTime >= delayTime)
			state = AttackState::None;
		break;
	}

}

bool EnemyAttackSystem::IsAttackAble()
{
	if (state != AttackState::None)
		return false;
	
	return true;
}

void EnemyAttackSystem::OnAttack()
{
	state = AttackState::Ready;
	runningTime = 0.0f;
}

Transform * EnemyAttackSystem::GetTransform()
{
	return system->GetTransform();
}

void EnemyAttackSystem::SetTag(wstring value)
{
	system->SetTag(value);
}
