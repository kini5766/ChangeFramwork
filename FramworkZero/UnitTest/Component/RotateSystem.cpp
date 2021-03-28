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
	// ���� ����
	float rad = acosf(dDot);

	// ���� ������ ���� ���
	if (rad <= FLT_EPSILON) 
	{
		if (outRad != nullptr)
			*outRad = rad;

		return q;
	}

	// ���� ������ ������ ���� ���� ���� ���
	if (rad < speedDelta)
	{
		// ���� ����(��)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &direction);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// ��ǥ ȸ��
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, rad);

		if (outRad != nullptr)
			*outRad = 0.0f;

		return q * dQ;
	}

	// ���� ����(��)
	Vector3 dAxis;
	D3DXVec3Cross(&dAxis, &forward, &direction);
	D3DXVec3Normalize(&dAxis, &dAxis);

	// speedDelta��ŭ ȸ��
	Quaternion dQ;
	D3DXQuaternionRotationAxis(&dQ, &dAxis, speedDelta);

	if (outRad != nullptr)
		*outRad = rad - speedDelta;

	return q * dQ;
}

