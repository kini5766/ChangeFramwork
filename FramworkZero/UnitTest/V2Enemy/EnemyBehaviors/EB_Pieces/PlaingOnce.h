#pragma once

#include "EnemyBehaviorPieceDesc.h"

// 힛 리액션이 종료 되면 리턴 호출
class PlaingOnce : public FlowTesk
{
public:
	PlaingOnce(const PlaingOnceDesc& desc);
	~PlaingOnce();

public:
	// IDelayFunction을(를) 통해 상속됨
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