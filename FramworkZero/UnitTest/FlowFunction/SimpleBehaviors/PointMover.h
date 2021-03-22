#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 지정 위치로 이동
class PointMover : public IFlowTesk
{
public:
	PointMover(const PointMoverDesc& desc);
	~PointMover();

	PointMoverDesc* GetDesc() { return &desc; }

private:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	PointMoverDesc desc;
	FutureReturn result;
};