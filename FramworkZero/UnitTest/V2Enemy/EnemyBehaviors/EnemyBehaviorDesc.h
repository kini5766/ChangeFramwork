#pragma once

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorsValues.h"

struct PatrollingDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;

	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;

	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// �ȱ� �ӵ�
	float* WalkSpeed;
	// �ѷ����� �ð�
	float LookAroundTime;

	// �� ���� �ý���
	class PerceptionSystem* Perceptor;

	// �� ���� �� ȣ��
	function<void()> FuncInRange;


public:
	PointMoverDesc MakeMover(int pointIndex);
	WaiterDesc MakeWaiter();
};


struct StrafeAroundDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// �޸��� Ŭ����ȣ
	UINT ClipRun;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// ��� Ŭ����ȣ
	UINT ClipIdle;

	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// �ȱ� �ӵ�
	float* WalkSpeed;
	// �޸��� �ӵ�
	float* RunSpeed;

	// �� ���� �ý���
	class PerceptionSystem* Perceptor;

	// �Ÿ� ���� ����
	float MinRangeSq;
	// �i�ƿ��� ����
	float ApproachRangeSq;

	// �� ��ġ�� ȣ��
	function<void()> FuncOutRange;

public:
	PointMoverDesc MakeMover();
	WaiterDesc MakeWaiter();
	FollowerDesc MakeFollower();
};


struct CombatPostureDesc
{
	// ��ų ��� �ƴ� �� �ൿ
	StrafeAroundDesc StrafeAroundDesc;

	// �� ��ų
	class IEnemySkill* Skill;
};


