#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Watchful : public FlowTesk
{
public:
	Watchful(const WatchfulDesc& desc);
	~Watchful();

public:
	// IDelayFunction을(를) 통해 상속됨
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	WatchfulDesc desc;

	FutureReturn result;
};
