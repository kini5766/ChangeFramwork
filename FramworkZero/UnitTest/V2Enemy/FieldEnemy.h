#pragma once

#include "FieldEnemyInput.h"

/*
����>[���ν�]>��Ȯ��>[�����]>����
����>[��ų��밡�ɽ�]>��ų>[�����]>����
����>[����ħ]>�ǵ��ư���>[�����]>����

���ݹ���, �������� �߰��� ��������
*/
class FieldEnemy
{
public:
	FieldEnemy(const FieldEnemyInput& input);
	~FieldEnemy();

public:
	void Update();

private:
	void InRangeDetection();
	function<void()> inRangeDetection;

private:
	FieldEnemyDesc desc;
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
