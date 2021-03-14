#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Patrol : public IDelayFunction
{
public:
	Patrol(const PatrolDesc& desc);
	~Patrol();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	PatrolDesc desc;

	DelayReturn result;
};
