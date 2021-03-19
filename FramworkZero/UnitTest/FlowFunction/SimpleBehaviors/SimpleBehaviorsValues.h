#pragma once

#include "stdafx.h"


struct WaiterDesc
{
	// ����� �ð�
	float Time;

	// �� �Լ��� �۵� �� �ִϸ��̼� ���
	function<void()> CallAnim = []() {};
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
	// ��ǥ����
	Vector3 Point;

	// �� �Լ��� �۵� �� �ִϸ��̼� ���
	function<void()> CallAnim = []() {};
};

struct FollowerDesc
{
	// �̵� �ý���
	class PointMoveSystem* MovingSystem;
	// �̵� �ӵ�
	float* MoveSpeed;

	// ��ǥ ����
	Vector3* Focus;

	// �� �Լ��� �۵� �� �ִϸ��̼� ���
	function<void()> CallAnim = []() {};
};


