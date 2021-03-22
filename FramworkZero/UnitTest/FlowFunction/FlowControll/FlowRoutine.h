#pragma once

/*
FlowTesk들을 미리 등록하고
호출 받으면 처음부터 재생
*/
class FlowRoutine : public FlowTesk
{
public:
	FlowRoutine();
	virtual ~FlowRoutine();

public:
	void Call(const FutureAction * action);
	void Update();
	void Cancel();

public:
	vector<FlowTesk*>* GetTesks() { return &tesks; }

private:
	FlowReader* reader;
	vector<FlowTesk*> tesks;
};
