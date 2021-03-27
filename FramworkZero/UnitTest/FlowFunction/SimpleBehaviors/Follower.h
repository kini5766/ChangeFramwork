#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Focus�� ��ġ�� �̵� (���ѹݺ�)
class Follower : public IFlowTesk
{
public:
	Follower(const FollowerDesc& desc);
	~Follower();

private:
	virtual void Call(const FutureAction * result) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	FollowerDesc desc;

	FutureReturn result;
};
