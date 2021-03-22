#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 클립 실행 하고 종료 되면 리턴 호출
class ClipPlayer : public IFlowTesk
{
public:
	ClipPlayer(const ClipPlayerDesc& desc);
	~ClipPlayer();

private:
	virtual void Call(const FutureAction* future) override;

private:
	ClipPlayerDesc desc;
	FutureReturn result;
};
