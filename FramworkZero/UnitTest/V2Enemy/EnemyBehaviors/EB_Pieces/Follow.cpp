#include "stdafx.h"
#include "Follow.h"

#include "Component/PointMoveSystem.h"

Follow::Follow(const FollowDesc& desc)
	: desc(desc)
{
}

Follow::~Follow()
{
}

void Follow::Call(const ReturnAction * action)
{
	desc.Animator->Play(*desc.ClipRun);
	result.SetAction(action);
}

void Follow::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(*desc.Focus, &lengthSq);

	if (lengthSq < (*desc.FollowInRangeSq))
	{
		// �ֺ��� ����
		result.OnAction();
		return;
	}

	desc.Target->SetMoveSpeed(*desc.RunSpeed);
	if (desc.Target->Update())
	{
		// ��Ȯ�� ����
		result.OnAction();
		return;
	}
}

void Follow::Cancel()
{
	result.Clear();
}
