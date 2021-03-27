#pragma once

#include "V2EnemyDesc.h"

/*
����>[���ν�]>��Ȯ��>[�����]>����
����>[��ų��밡�ɽ�]>��ų>[�����]>����
����>[����ħ]>�ǵ��ư���>[�����]>����

���ݹ���, �������� �߰��� ��������
*/
class FieldEnemy
{
public:
	FieldEnemy(const FieldEnemyInput& desc);
	~FieldEnemy();


public:
	void Update();
	void PostRender();

private:
	void OnDamage();
	void OnLost();
	function<void()> onLost;
	void InRange();

private:
	V2EnemyStatus oriStatus;  // ���� �ɷ�ġ
	V2EnemyStatus status;  // ���� �ɷ�ġ
	class IFocus* player;  // ���� ��� (��ġ)
	Transform* transform;  // �� ��ġ
	Animator* animator;  // �� �ִϸ�����
	vector<Vector3> patrolPoints;  // ���� ���

	class HPSystem* hp;  // ü��, ���� �޴� ����
	//FieldEnemyClipNumGroup clipGroup;

	bool bDead = false;
	bool bLost = false;  // �׾ �� ���̴� ����


	// -- Field Behavior Service -- //
private:
	class PerceptionSystem* perceptor;
	class PointMoveSystem* movingSystem;

	// -- Field Behavior -- //
private:
	class FlowReader* reader;

	class Patrolling* pat;

	class FlowRoutine* recall;
	class FlowAction* playRun;
	class PointMover* moveto;

	class CombatPosture* combat;

	class ClipOncePlayer* clipInSight;
	class ClipOncePlayer* clipHitted;
	class ClipOncePlayer* clipDead;
};


// ���� ��
// �� ����
// �� �پ�ȭ