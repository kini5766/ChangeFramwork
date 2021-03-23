#pragma once

/*
�Լ� ����ϰ� ����
*/
class FlowAction : public IFlowTesk
{
public:
	FlowAction(const FutureAction& action);
	virtual ~FlowAction();

public:
	virtual void Call(const FutureAction * result) override;

private:
	FutureAction action;
};