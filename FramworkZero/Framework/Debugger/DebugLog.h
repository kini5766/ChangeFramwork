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
	// IDebugLog��(��) ���� ��ӵ�
	void Print(string log) override;

public:
	void Render();

private:
	list<string> logs;
};