#pragma once

#include "EnemyBehaviorPieceDesc.h"

// 힛 리액션이 종료 되면 리턴 호출
class PlaingOnce : public IDelayFunction
{
public:
	PlaingOnce(const PlaingOnceDesc& desc);
	~PlaingOnce();

public:
	// IDelayFunction을(를) 통해 상속됨
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