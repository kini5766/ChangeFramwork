#pragma once

#include "FlowFunction/FlowFunction.h"
#include "SimpleBehaviorsValues.h"


// Ŭ�� �����ϰ� ���� ȣ��
class ClipOncePlayer : public IFlowTesk
{
public:
	ClipOncePlayer(const ClipPlayerDesc& desc);
	~ClipOncePlayer();

private:
	virtual void Call(const FutureAction * result) override;
	virtual void Update() override;
	virtual void Cancel() override;

private:
	void CompleAnim();

private:
	ClipPlayerDesc desc;
	function<void()> funcComple;

	bool bChanged = false;
	FutureReturn result;
};