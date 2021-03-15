#pragma once

class FlowBase
{
public:
	enum class ClassType
	{
		None = 0,
		FlowAction,
		FlowTesk
	} GetType() { return classType; }

protected:
	ClassType classType = ClassType::None;

public:
	// 다이나믹 캐스트용
	virtual ~FlowBase() {}
};
