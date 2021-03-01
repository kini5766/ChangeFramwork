#pragma once

#include "EnemyBehaviorPieceDesc.h"


class LookAround : public IDelayFunction
{
public:
	LookAround(const LookAroundDesc& desc);
	~LookAround();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	void Call(const DelayReturn * result) override;
	void Update() override;
	void Cancel() override;

private:
	LookAroundDesc desc;
	float runningTime = 0.0f;
	const DelayReturn* result;
};