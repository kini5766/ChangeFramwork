#include "stdafx.h"
#include "TimeTable.h"

TimeTable::TimeTable()
{
}

TimeTable::~TimeTable()
{
	for (auto d : nodes)
		SafeDelete(d);
}

void TimeTable::Rewind()
{
	curr = 0;
	runningTime = 0.0f;
}

void TimeTable::UpdateTime()
{
	if (nodes.size() == 0)
		return;

	if (nodes[curr]->DelayTime < runningTime)
	{
		runningTime -= nodes[curr]->DelayTime;
		curr = (curr++) % nodes.size();
		nodes[curr]->Action();
	}

	runningTime += Time::Delta();
}

TimeTableNode * TimeTable::GetCurrNode()
{
	if (nodes.size() == 0)
		return nullptr;

	return nodes[curr];
}

void TimeTable::AddNode(const TimeTableNode & value)
{
	TimeTableNode* node = new TimeTableNode(value);
	nodes.push_back(node);
}
