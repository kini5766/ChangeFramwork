#pragma once

#include "FieldBehaviorInput.h"

/*
순찰>[적인식]>적확인>[종료시]>전투
전투>[스킬사용가능시]>스킬>[종료시]>전투
전투>[적놓침]>되돌아가기>[종료시]>순찰

공격받음, 쓰러짐은 중간에 끼워넣음
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
	void InRangeDetection();
	function<void()> inRangeDetection;

private:
	FieldBehaviorDesc desc;
	Transform* transform;
	class IFocus* player;

private:
	Vector3 minePosition;
	Vector3 focusPosition;

private:
	class DelayReader* reader;
	class Patrolling* pat;
	class PlaingOnce* detection;
};
