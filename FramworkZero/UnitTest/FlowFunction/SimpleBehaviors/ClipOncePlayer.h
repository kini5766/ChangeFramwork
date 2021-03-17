#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Ŭ�� �����ϰ� ���� ȣ��
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