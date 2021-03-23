#pragma once

/*
함수 재생하고 종료
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