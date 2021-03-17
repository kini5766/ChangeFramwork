#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Focus의 위치로 이동
class Follower : public FlowTesk
{
public:
	Follower(const FollowerDesc& desc);
	~Follower();

private:
	void Call(const FutureAction * result);
	void Update();
	void Cancel();

private:
	FollowerDesc desc;

	FutureReturn result;
};
