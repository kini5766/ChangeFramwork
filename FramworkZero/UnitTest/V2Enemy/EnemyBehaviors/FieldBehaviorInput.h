#pragma once

struct FieldBehaviorInput
{
	Transform* Trans;

	// ����� �ִϸ�����
	Animator* Anim;
	// ��� Ŭ����ȣ
	UINT ClipIdle;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �޸��� Ŭ����ȣ todo
	UINT ClipRun;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;
	// �� �߰� Ŭ����ȣ
	UINT ClipInSight;


	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;

	// todo
	class IEnemySkill* Skill;
};



#include "EnemyBehaviorDesc.h"
#include "Rendering/Camera/Main/IFocus.h"

struct FieldBehaviorDesc
{
public:
	FieldBehaviorDesc(const FieldBehaviorInput& input);
	~FieldBehaviorDesc();

public:
	// ����� �ִϸ�����
	Animator* Anim;
	// ��� Ŭ����ȣ
	UINT ClipIdle;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �޸��� Ŭ����ȣ
	UINT ClipRun;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;
	// �� �߰� Ŭ����ȣ
	UINT ClipInSight;

	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;

	// �� ��ų
	class IEnemySkill* Skill;


	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// [��������] �ȱ� �ӵ�
	float WalkSpeed = 3.0f;
	// [��������] �޸��� �ӵ�
	float RunSpeed = 10.0f;

	// [��������] �� ���� �ý���
	class PerceptionSystem* Perceptor = nullptr;
	// [��������] �� ���� ���� (����)
	float SightRangeSq = 400.0f;
	// [��������] �� ��ħ ���� (����)
	float LoseRangeSq = 900.0f;
	// [��������] �i�ƿ��� ����
	float ApproachRangeSq = 400.0f;
	// [��������] �Ÿ� ������ ����
	float MinRange = 10.0f;

	// [��������] �ѷ����� �ð�
	float LookAroundTime = 1.0f;

	// [��������] ���� �ȿ� ������ ��
	function<void()> FuncInRange;
	// [��������] ���� �ۿ� ������ ��
	function<void()> FuncOutRange;


public:
	operator PatrollingDesc();

	ClipPlayerDesc MakeInSight();
	PointMoverDesc MakeComeback();
	CombatPostureDesc MakeCombat();
};
