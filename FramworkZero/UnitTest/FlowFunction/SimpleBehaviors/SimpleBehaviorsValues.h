#pragma once

#include "stdafx.h"


struct WaiterDesc
{
	// ����� �ð�
	float Time;
};


struct ClipPlayerDesc
{
	// ����� �ִϸ�����
	Animator* Anim;
	// ����� Ŭ��
	UINT Clip;
};


struct PointMoverDesc
{
	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// �̵� �ӵ�
	float* WalkSpeed;
	// ��ǥ������ ���� (����)
	float* PatrolSafeRangeSq;
	// ��ǥ����
	Vector3 Point;
};

struct FollowerDesc
{
	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// �޸��� �ӵ�
	float* RunSpeed;

	// ��ǥ������ ���� (����)
	float* FollowInRangeSq;
	// ��ǥ ����
	Vector3* Focus;
};


