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

void PointMoveSystem::PreUpdate(const Vector3& point, float * outLengthSq)
{
	this->point = point;

	Vector3 position;
	transform->Position(&position);
	destance = point - position;
	destance.y = 0.0f;
	D3DXVec3Normalize(&direction, &destance);

	if (outLengthSq != nullptr)
		*outLengthSq = D3DXVec3LengthSq(&destance);
}

bool PointMoveSystem::Update()
{
	float speedDelta = movingSpeed * Time::Delta();

	// ȸ��
	Quaternion q;
	transform->Rotation(&q);
	sysRatate->SetTarget(direction);
	q = sysRatate->GetRotation(q, -transform->Forward(), speedDelta * rotateSpeed);
	transform->Rotation(q);

	// �̵�
	Vector3 position;
	transform->Position(&position);
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
