#pragma once

#include "FieldBehaviorInput.h"

/*
����>[���ν�]>��Ȯ��>[�����]>����
����>[��ų��밡�ɽ�]>��ų>[�����]>����
����>[����ħ]>�ǵ��ư���>[�����]>����

���ݹ���, �������� �߰��� ��������
*/
class FieldBehavior
{
public:
	FieldBehavior(const FieldBehaviorInput& input);
	~FieldBehavior();

public:
	void Update();

public:
	void SetFocus(class IFocus* value) { player = value; }


private:
	void InRange();
	function<void()> inRange;
	//void OutRange();
	//function<void()> outRange;

private:
	FieldBehaviorDesc desc;
	Transform* transform;
	class IFocus* player;

private:
	Vector3 minePosition;
	Vector3 focusPosition;

private:
	class FlowReader* reader;

	class Patrolling* pat;

	class FlowRoutine* recall;
	class FlowAction* playRun;
	class PointMover* moveto;

	class CombatPosture* combat;

	class ClipOncePlayer* clipInSight;
};
