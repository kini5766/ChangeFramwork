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
	// ���� ����
	float rad = acosf(dDot);
	if (outRad != nullptr)
		*outRad = rad;

	// ���� ������ ���� ���
	if (rad <= FLT_EPSILON) 
	{
		return q;
	}
	else if (rad < speedDelta)
	{
		// ���� ����(��)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &target);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// ��ǥ ȸ��
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, rad);

		return q * dQ;
	}
	else
	{
		// ���� ����(��)
		Vector3 dAxis;
		D3DXVec3Cross(&dAxis, &forward, &target);
		D3DXVec3Normalize(&dAxis, &dAxis);

		// speedDelta��ŭ ȸ��
		Quaternion dQ;
		D3DXQuaternionRotationAxis(&dQ, &dAxis, speedDelta);

		return q * dQ;
	}
	return Quaternion();
}

