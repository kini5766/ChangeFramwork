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
	float dDot = D3DXVec3Dot(&target, &forward);
	// 남은 방향
	float rad = acosf(dDot);
	if (outRad != nullptr)
		*outRad = rad;

	// 남은 방향이 없을 경우
	if (rad <= FLT_EPSILON) 
	{
		return q;
	}
	else if (rad < speedDelta)
	{
		// 남은 방향(축)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &target);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// 목표 회전
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, rad);

		return q * dQ;
	}
	else
	{
		// 남은 방향(축)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &target);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// speedDelta만큼 회전
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, speedDelta);

		return q * dQ;
	}
	return Quaternion();
}

