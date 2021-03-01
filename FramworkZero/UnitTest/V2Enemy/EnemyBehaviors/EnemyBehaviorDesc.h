#pragma once

#include "EB_Pieces/EnemyBehaviorPieceDesc.h"

struct PatrollingDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	UINT* ClipWalk;
	// �ѷ����� Ŭ����ȣ
	UINT* ClipLookAround;

	// ��ǥ������
	const Vector3* PatrolPoints;
	// ��ǥ��������
	UINT PatrolCount;

	// �̵� �ý���
	class PointMoveSystem* Target;
	// ��ǥ������ ���� (����)
	float* PatrolSafeRangeSq;
	// �̵� �ӵ�
	float* WalkSpeed;
	// �ѷ����� �ð�
	float* LookAroundTime;

	// �� ���� �ý���
	class EnemyDetectionSystem* DetectionSystem;

public:
	PatrolDesc CastPatrol(int pointIndex);
	operator LookAroundDesc();
};
