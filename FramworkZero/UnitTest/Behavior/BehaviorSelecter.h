#pragma once

// ã�� : Boundary range
// ���� ���� ���� : Attack range

// ���� �� : InBattle

typedef function<bool()> IsAble;

class BehaviorSelecter
{
public:
	BehaviorSelecter();
	~BehaviorSelecter();

public:
	UINT GetNextBehavior();

public:
	vector<IsAble>* GetBehaviors() { return &behaviors; }

private:
	vector<IsAble> behaviors;
};
