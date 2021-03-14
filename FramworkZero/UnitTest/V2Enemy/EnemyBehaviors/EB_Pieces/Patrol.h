#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Patrol : public IDelayFunction
{
public:
	Patrol(const PatrolDesc& desc);
	~Patrol();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	PatrolDesc desc;

	DelayReturn result;
};
