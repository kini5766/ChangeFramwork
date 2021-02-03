#pragma once

struct TimeTableNode {
	wstring Name = L"None";
	float DelayTime = 0.0f;
	function<void(void)> Action = [](){};
};

class TimeTable
{
public:
	TimeTable();
	~TimeTable();

public:
	void Rewind();
	void UpdateTime();

public:
	TimeTableNode* GetCurrNode();
	void AddNode(const TimeTableNode& value);

private:
	float runningTime = 0.0f;
	UINT curr = 0;
	vector<TimeTableNode*> nodes;
};