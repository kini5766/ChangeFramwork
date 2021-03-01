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

void Follow::Call(const DelayReturn * _result)
{
	desc.Animator->Play(*desc.ClipRun);
	result = _result;
}

void Follow::Update()
{
	float lengthSq;
	desc.Target->PreUpdate(*desc.Focus, &lengthSq);

	if (lengthSq < (*desc.FollowInRangeSq))
	{
		// �ֺ��� ����
		(*result)();
		result = nullptr;
		return;
	}

	desc.Target->SetMoveSpeed(*desc.RunSpeed);
	if (desc.Target->Update())
	{
		// ��Ȯ�� ����
		(*result)();
		result = nullptr;
		return;
	}
}

void Follow::Cancel()
{
	result = nullptr;
}
