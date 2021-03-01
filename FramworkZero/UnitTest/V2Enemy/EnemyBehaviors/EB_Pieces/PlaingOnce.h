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
	void CompleAnim(UINT i);

private:
	PlaingOnceDesc desc;
	function<void(UINT)> funcComple;

	const DelayReturn* result;
};