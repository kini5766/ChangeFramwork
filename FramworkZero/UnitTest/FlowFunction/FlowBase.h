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
	// ���̳��� ĳ��Ʈ��
	virtual ~FlowBase() {}
};
