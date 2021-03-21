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
	// 위치
	Vector3 position;
	transform->Position(&position);

	// 거리
	Vector3 destance = point - position;
	destance.y = 0.0f;

	// 방향
	Vector3 direction;
	D3DXVec3Normalize(&direction, &destance);

	float lengthSq = D3DXVec3LengthSq(&destance);
	return lengthSq < safeRangeSq;
}

bool PointMoveSystem::GoToPoint()
{
	// 위치
	Vector3 position;
	transform->Position(&position);

	// 거리
	Vector3 destance = point - position;
	destance.y = 0.0f;

	// 방향
	Vector3 direction;
	D3DXVec3Normalize(&direction, &destance);

	float lengthSq = D3DXVec3LengthSq(&destance);
	if (lengthSq < safeRangeSq)
	{
		// 주변에 도착
		return true;
	}


	if (speeder == nullptr)
	// 주변과 떨어져있는데 이동 속도가 0인 경우
		return false;

	float speedDelta = (*speeder) * Time::Delta();

	// 회전
	Quaternion q;
	transform->Rotation(&q);
	sysRatate->SetTarget(direction);
	q = sysRatate->GetRotation(q, -transform->Forward(), speedDelta * rotateSpeed);
	transform->Rotation(q);

	// 이동
	position += -transform->Forward() * speedDelta;
	transform->Position(position);


	if (lengthSq < speedDelta * speedDelta)
	{
		// 정확히 도착
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
