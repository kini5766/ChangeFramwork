#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Watchful : public IDelayFunction
{
public:
	Watchful(const WatchfulDesc& desc);
	~Watchful();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	WatchfulDesc desc;

	DelayReturn result;
};
