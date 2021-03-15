#pragma once

#include "EnemyBehaviorPieceDesc.h"

// �� ���׼��� ���� �Ǹ� ���� ȣ��
class PlaingOnce : public FlowTesk
{
public:
	PlaingOnce(const PlaingOnceDesc& desc);
	~PlaingOnce();

public:
	// IDelayFunction��(��) ���� ��ӵ�
	virtual void Call(const FutureAction * result);
	virtual void Cancel();

private:
	void CompleAnim();

private:
	PlaingOnceDesc desc;
	function<void()> funcComple;

	bool bChanged = false;
	FutureReturn result;
};