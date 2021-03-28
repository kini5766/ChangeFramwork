#include "stdafx.h"
#include "RotateSystem.h"

RotateSystem::RotateSystem()
{
}

RotateSystem::~RotateSystem()
{
}

Quaternion RotateSystem::GetRotation(const Quaternion & q, const Vector3 & forward, float speedDelta, float* outRad)
{
	float dDot = D3DXVec3Dot(&direction, &forward);
	// 남은 방향
	float rad = acosf(dDot);

	// 남은 방향이 없을 경우
	if (rad <= FLT_EPSILON) 
	{
		if (outRad != nullptr)
			*outRad = rad;

		return q;
	}

	// 남은 방향이 움직일 방향 보다 작은 경우
	if (rad < speedDelta)
	{
		// 남은 방향(축)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &direction);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// 목표 회전
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, rad);

		if (outRad != nullptr)
			*outRad = 0.0f;

		return q * dQ;
	}

	// 남은 방향(축)
	Vector3 dAxis;
	D3DXVec3Cross(&dAxis, &forward, &direction);
	D3DXVec3Normalize(&dAxis, &dAxis);

	// speedDelta만큼 회전
	Quaternion dQ;
	D3DXQuaternionRotationAxis(&dQ, &dAxis, speedDelta);

	if (outRad != nullptr)
		*outRad = rad - speedDelta;

	return q * dQ;
}

