#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 클립 실행 하고 종료 되면 리턴 호출
class ClipPlayer : public FlowTesk
{
public:
	ClipPlayer(const ClipPlayerDesc& desc);
	~ClipPlayer();

private:
	void Call(const FutureAction* future);

private:
	ClipPlayerDesc desc;
	FutureReturn result;
};
