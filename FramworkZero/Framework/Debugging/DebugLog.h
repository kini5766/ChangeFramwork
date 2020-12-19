#pragma once

class IDebugLog
{
public:
	virtual void Print(string log) = 0;
};

class DebugLog : public IDebugLog
{
public:
	DebugLog();
	~DebugLog();

public:
	// IDebugLog을(를) 통해 상속됨
	void Print(string log) override;

public:
	void Render();

private:
	list<string> logs;
};