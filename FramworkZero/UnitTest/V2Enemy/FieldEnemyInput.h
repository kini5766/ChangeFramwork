#pragma once

struct FieldEnemyInput
{
	Transform* Trans;

	// ����� �ִϸ�����
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;
	// �� �߰� Ŭ����ȣ
	UINT ClipInSight;


	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;
};



#include "EnemyBehaviors/EnemyBehaviorDesc.h"
#include "Rendering/Camera/Main/IFocus.h"

struct FieldEnemyDesc
{
public:
	FieldEnemyDesc(const FieldEnemyInput& input);
	~FieldEnemyDesc();

public:
	// ����� �ִϸ�����
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	UINT ClipWalk;
	// �ѷ����� Ŭ����ȣ
	UINT ClipLookAround;
	// �� �߰� Ŭ����ȣ
	UINT ClipInSight;

	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;

	// �̵� �ý���
	class PointMoveSystem* MoveSystem; 
	// [��������] �ȱ� �ӵ�
	float WalkSpeed = 3.0f;

	// [��������] �� ���� �ý���
	class EnemyDetectionSystem* DetectionSystem = nullptr;

	// [��������] �ѷ����� �ð�
	float LookAroundTime = 1.0f;
	// [��������] ��ǥ������ ���� (����)
	float PatrolSafeRangeSq = 1.0f;

public:
	operator PatrollingDesc();
};
