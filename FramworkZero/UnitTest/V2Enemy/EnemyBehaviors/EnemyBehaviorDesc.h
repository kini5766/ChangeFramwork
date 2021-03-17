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
	// ��ǥ������ ���� (����)
	float* PatrolSafeRangeSq;
	// �̵� �ӵ�
	float* WalkSpeed;
	// �ѷ����� �ð�
	float LookAroundTime;

	// �� ���� �ý���
	class EnemyDetectionSystem* DetectionSystem;

public:
	ClipPlayerDesc MakeWalk();
	PointMoverDesc MakeMover(int pointIndex);
	ClipPlayerDesc MakeLookAround();
	WaiterDesc MakeWaiter();
};
