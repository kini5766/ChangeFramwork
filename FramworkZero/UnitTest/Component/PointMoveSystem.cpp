#include "stdafx.h"
#include "PointMoveSystem.h"

#include "Component/RotateSystem.h"

PointMoveSystem::PointMoveSystem(Transform * transform)
	: transform(transform)
{
	sysRatate = new RotateSystem();
}

PointMoveSystem::~PointMoveSystem()
{
	SafeDelete(sysRatate);
}

bool PointMoveSystem::IsAround()
{
	// ��ġ
	Vector3 position;
	transform->Position(&position);

	// �Ÿ�
	Vector3 destance = point - position;
	destance.y = 0.0f;

	// ����
	Vector3 direction;
	D3DXVec3Normalize(&direction, &destance);

	float lengthSq = D3DXVec3LengthSq(&destance);
	return lengthSq < safeRangeSq;
}

bool PointMoveSystem::GoToPoint()
{
	// ��ġ
	Vector3 position;
	transform->Position(&position);

	// �Ÿ�
	Vector3 destance = point - position;
	destance.y = 0.0f;

	// ����
	Vector3 direction;
	D3DXVec3Normalize(&direction, &destance);

	float lengthSq = D3DXVec3LengthSq(&destance);
	if (lengthSq < safeRangeSq)
	{
		// �ֺ��� ����
		return true;
	}


	if (speeder == nullptr)
	// �ֺ��� �������ִµ� �̵� �ӵ��� 0�� ���
		return false;

	float speedDelta = (*speeder) * Time::Delta();

	// ȸ��
	Quaternion q;
	transform->Rotation(&q);
	sysRatate->SetTarget(direction);
	q = sysRatate->GetRotation(q, -transform->Forward(), speedDelta * rotateSpeed);
	transform->Rotation(q);

	// �̵�
	position += -transform->Forward() * speedDelta;
	transform->Position(position);


	if (lengthSq < speedDelta * speedDelta)
	{
		// ��Ȯ�� ����
		transform->Position(point);
		return true;
	}

	return false;
}

float PointMoveSystem::GetLengthSq()
{
	Vector3 position;
	transform->Position(&position);
	Vector3 destance = point - position;
	destance.y = 0.0f;

	return D3DXVec3LengthSq(&destance);
}
