#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Watchful : public FlowTesk
{
public:
	Watchful(const WatchfulDesc& desc);
	~Watchful();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	WatchfulDesc desc;

	FutureReturn result;
};
