#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Patrol : public IDelayFunction
{
public:
	Patrol(const PatrolDesc& desc);
	~Patrol();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	PatrolDesc desc;

	const DelayReturn* result;
};
