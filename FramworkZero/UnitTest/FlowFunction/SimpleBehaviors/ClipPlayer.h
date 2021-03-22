#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Ŭ�� ���� �ϰ� ���� �Ǹ� ���� ȣ��
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
