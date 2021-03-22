#pragma once

/*
작업들을 미리 등록하고
호출 받으면 처음부터 재생
*/
class FlowRoutine : public IFlowTesk
{
public:
	FlowRoutine();
	virtual ~FlowRoutine();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;

public:
	vector<IFlowTesk*>* Tesks() { return &tesks; }

private:
	FlowReader* reader;
	vector<IFlowTesk*> tesks;
};
