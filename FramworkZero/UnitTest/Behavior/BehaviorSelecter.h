#pragma once

// 찾기 : Boundary range
// 공격 가능 범위 : Attack range

// 전투 중 : InBattle

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
