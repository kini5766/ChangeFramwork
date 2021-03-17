#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 클립 실행하고 리턴 호출
class ClipOncePlayer : public FlowTesk
{
public:
	ClipOncePlayer(const ClipPlayerDesc& desc);
	~ClipOncePlayer();

private:
	void Call(const FutureAction * result);
	void Cancel();

private:
	void CompleAnim();

private:
	ClipPlayerDesc desc;
	function<void()> funcComple;

	bool bChanged = false;
	FutureReturn result;
};