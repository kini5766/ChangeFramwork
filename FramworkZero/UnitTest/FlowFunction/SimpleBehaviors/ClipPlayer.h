#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Ŭ�� ���� �ϰ� ���� �Ǹ� ���� ȣ��
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
