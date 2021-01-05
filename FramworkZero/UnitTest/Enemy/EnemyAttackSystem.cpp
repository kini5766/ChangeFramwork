#include "stdafx.h"
#include "EnemyAttackSystem.h"

EnemyAttackSystem::EnemyAttackSystem(IFocus * player)
	: player(player)
{
	sendbox = new SendBox(CollisionManager::Get()->CreateCollider());
	sendbox->SetActiveSelf(false);
	sendbox->SetSendMessageData(&attack);
}

EnemyAttackSystem::~EnemyAttackSystem()
{
	SafeDelete(sendbox);
}

Transform * EnemyAttackSystem::GetTransform()
{
	return sendbox->GetTransform();
}

void EnemyAttackSystem::SetTag(wstring value)
{
	sendbox->SetTag(value);
}

void EnemyAttackSystem::StartAttack()
{
	sendbox->SetActiveSelf(true);
	sendbox->OnSendMessage();
}

void EnemyAttackSystem::EndAttack()
{
	sendbox->SetActiveSelf(false);
}


void EnemyAttackSystem::Update()
{
	if (sendbox->IsActiveSelf())
		Debug::Box->RenderBox(sendbox->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
	
	switch (state)
	{
	case EnemyAttackSystem::AttackState::None:
		break;
	case EnemyAttackSystem::AttackState::Ready:
		runningTime += Time::Delta();
		if (runningTime >= readyTime)
		{
			StartAttack();
			state = AttackState::Attack;
		}
		break;
	case EnemyAttackSystem::AttackState::Attack:
		runningTime += Time::Delta();
		if (runningTime >= endTime)
		{
			EndAttack();
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

bool EnemyAttackSystem::IsAttackAble(const Vector3& position)
{
	if (state != AttackState::None)
		return false;

	Vector3 focus;
	player->Focus(&focus);
	float a = D3DXVec3LengthSq(&(focus - position));

	if (a > range * range)
		return false;

	return true;
}

void EnemyAttackSystem::OnAttack()
{
	state = AttackState::Ready;
	runningTime = 0.0f;
}
