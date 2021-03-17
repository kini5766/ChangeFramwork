#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 지정 시간 만큼 대기
class Waiter : public FlowTesk
{
public:
	Waiter(const WaiterDesc& desc);
	~Waiter();

private:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	WaiterDesc desc;
	float runningTime = 0.0f;

	FutureReturn result;
};
