#pragma once

#include "FlowFunction/FlowFunction.h"


struct PlaingOnceDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// ����� Ŭ��
	UINT Clip;
};

// --
// Patrolling
// --
struct MovingToPointDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	UINT* ClipWalk;

	// �̵� �ý���
	class PointMoveSystem* Target;
	// �̵� �ӵ�
	float* WalkSpeed;
	// ��ǥ������ ���� (����)
	float* PatrolSafeRangeSq;
	// ��ǥ����
	Vector3 Point;
};
struct LookAroundDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// �ѷ����� Ŭ��
	const UINT* ClipLookAround;

	// �ѷ����� �ð�
	float* LookAroundTime;
};


// --
// InBattle
// --
struct FollowDesc
{
	// �̵� �ý���
	class PointMoveSystem* Target;
	// �޸��� �ӵ�
	float* RunSpeed;

	// �ִϸ����� ���
	Animator* Animator;
	// �޸��� Ŭ����ȣ
	UINT* ClipRun;

	// ��ǥ������ ���� (����)
	float* FollowInRangeSq;
	// ��ǥ ����
	Vector3* Focus;
};
struct WatchfulDesc
{
	// �̵� �ý���
	//class PointMoveSystem* Target;
	// �͸����� �ӵ�
	//float* WalkSpeed;

	// �ִϸ����� ���
	Animator* Anim;
	// �ȱ� Ŭ����ȣ
	//UINT* WalkClipNum;
	// ��� Ŭ����ȣ
	UINT* IdleClipNum;

	// �ּ� ������ �Ÿ�
	//float* InnerRange;
	// �ִ� ������ �Ÿ�
	//float* OuterRange;
	// ��ǥ ����
	//Vector3* Focus;
};