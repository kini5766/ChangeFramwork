#include "stdafx.h"
#include "AttackSystem.h"

AttackSystem::AttackSystem()
{
	sendbox = new SendBox(CollisionManager::Get()->CreateCollider());
	sendbox->SetActiveSelf(false);
	sendbox->SetSendMessageData(&attack);
}

AttackSystem::~AttackSystem()
{
	SafeDelete(sendbox);
}

void AttackSystem::StartAttack()
{
	sendbox->SetActiveSelf(true);
	sendbox->OnSendMessage();
}

void AttackSystem::EndAttack()
{
	sendbox->SetActiveSelf(false);
}

bool AttackSystem::IsAttacking()
{
	return sendbox->IsActiveSelf();
}

Transform * AttackSystem::GetTransform()
{
	return sendbox->GetTransform();
}

void AttackSystem::SetTag(wstring value)
{
	sendbox->SetTag(value);
}
