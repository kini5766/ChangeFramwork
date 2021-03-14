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
	virtual void Call(const ReturnAction * result) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	void CompleAnim();

private:
	PlaingOnceDesc desc;
	function<void()> funcComple;

	bool bChanged = false;
	DelayReturn result;
};