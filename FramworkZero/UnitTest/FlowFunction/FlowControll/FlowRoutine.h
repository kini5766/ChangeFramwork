#pragma once

/*
작업들을 미리 등록하고
호출 받으면 처음부터 재생
(일시중지 되어도 처음부터)
*/
class FlowRoutine : public IFlowTesk
{
public:
	FlowRoutine();
	virtual ~FlowRoutine();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Hold() override;

public:
	vector<IFlowTesk*>* Tesks() { return &tesks; }

private:
	FlowReader* reader;
	vector<IFlowTesk*> tesks;
};
