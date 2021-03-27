#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 클립 실행하고 리턴 호출
class ClipOncePlayer : public IFlowTesk
{
public:
	ClipOncePlayer(const ClipPlayerDesc& desc);
	~ClipOncePlayer();

private:
	virtual void Call(const FutureAction * result) override;
	virtual void Update() override;
	virtual void Hold() override;

private:
	void CompleAnim();

private:
	ClipPlayerDesc desc;
	function<void()> funcComple;

	bool bChanged = false;
	bool bCancel = false;
	FutureReturn result;
};