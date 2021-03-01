#pragma once

#include "EnemyBehaviorPieceDesc.h"

class Watchful : public IDelayFunction
{
public:
	Watchful(const WatchfulDesc& desc);
	~Watchful();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	WatchfulDesc desc;

	const DelayReturn* result;
};
