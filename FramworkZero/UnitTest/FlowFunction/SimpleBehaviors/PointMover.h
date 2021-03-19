#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// 지정 위치로 이동
class PointMover : public FlowTesk
{
public:
	PointMover(const PointMoverDesc& desc);
	~PointMover();

	PointMoverDesc* GetDesc() { return &desc; }

private:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

private:
	PointMoverDesc desc;
	FutureReturn result;
};