#pragma once

struct FieldBehaviorInput
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

public:
	operator PatrollingDesc();

	ClipPlayerDesc MakeInSight();
};
