#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Watchful : public IDelayFunction
{
public:
	Watchful(const WatchfulDesc& desc);
	~Watchful();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const ReturnAction * action) override;
	void Update() override;
	void Cancel() override;

private:
	WatchfulDesc desc;

	DelayReturn result;
};
