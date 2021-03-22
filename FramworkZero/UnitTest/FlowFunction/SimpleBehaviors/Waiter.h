#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 지정 시간 만큼 대기
class Waiter : public IFlowTesk
{
public:
	Waiter(const WaiterDesc& desc);
	~Waiter();

public:
	WaiterDesc* GetDesc() { return &desc; }

private:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	WaiterDesc desc;
	float runningTime = 0.0f;

	FutureReturn result;
};
