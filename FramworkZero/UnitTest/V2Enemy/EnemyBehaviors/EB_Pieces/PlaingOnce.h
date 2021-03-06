#pragma once

#include "EnemyBehaviorPieceDesc.h"

// �� ���׼��� ���� �Ǹ� ���� ȣ��
class PlaingOnce : public IDelayFunction
{
public:
	PlaingOnce(const PlaingOnceDesc& desc);
	~PlaingOnce();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	virtual void Call(const DelayReturn * result) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	void CompleAnim();

private:
	PlaingOnceDesc desc;
	function<void()> funcComple;

	const DelayReturn* result;
	bool bChanged;
};