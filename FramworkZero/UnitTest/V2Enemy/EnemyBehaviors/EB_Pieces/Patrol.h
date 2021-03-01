#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Patrol : public IDelayFunction
{
public:
	Patrol(const PatrolDesc& desc);
	~Patrol();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	PatrolDesc desc;

	const DelayReturn* result;
};
