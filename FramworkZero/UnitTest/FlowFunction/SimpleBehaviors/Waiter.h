#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// ���� �ð� ��ŭ ���
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
