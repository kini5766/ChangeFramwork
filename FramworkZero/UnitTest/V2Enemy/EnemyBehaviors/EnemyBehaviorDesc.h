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
	// �̵� �ӵ�
	float* WalkSpeed;
	// �ѷ����� �ð�
	float LookAroundTime;

	// �� ���� �ý���
	class EnemyDetectionSystem* DetectionSystem;

public:
	PointMoverDesc MakeMover(int pointIndex);
	WaiterDesc MakeWaiter();
};
