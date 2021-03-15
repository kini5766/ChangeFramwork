#pragma once

#include "EnemyBehaviorPieceDesc.h"


class Follow : public FlowTesk
{
public:
	Follow(const FollowDesc& desc);
	~Follow();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const FutureAction * result);
	void Update();
	void Cancel();

private:
	FollowDesc desc;

	FutureReturn result;
};