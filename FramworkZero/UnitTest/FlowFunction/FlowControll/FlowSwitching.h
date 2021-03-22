#pragma once

typedef function<int()> FuncCase;

class FlowSwitching : public IFlowTesk
{
public:
	FlowSwitching();
	virtual ~FlowSwitching();

public:
	virtual void Call(const FutureAction * action) override;
	virtual void Update() override;
	virtual void Cancel() override;

public:
	void SetFuncCase(const FuncCase& value) { funcCase = value; }

private:
	FutureReturn result;
	FuncCase funcCase = []() { return -1; };

	int curr = -1;

	vector<IFlowTesk*> tesks;
};