#include "stdafx.h"
#include "Follow.h"

#include "Component/RotateSystem.h"

Follow::Follow(const FollowDesc& desc)
	: desc(new FollowDesc(desc))
{
}

Follow::~Follow()
{
	SafeDelete(desc);
}

void Follow::Update()
{
	float speedDelta = (*desc->RunSpeed) * Time::Delta();

	Vector3 dest2 = *desc->Dest;
	dest2.y = 0.0f;
	D3DXVec3Normalize(&dest2, &dest2);

	// 회전
	desc->Ratate->SetTarget(dest2);
	Quaternion q;
	desc->Target->Rotation(&q);
	q = desc->Ratate->GetRotation(q, -desc->Target->Forward(), speedDelta * (*desc->TurnSpeed));
	desc->Target->Rotation(q);

	// 이동
	Vector3 position;
	desc->Target->Position(&position);
	position += -desc->Target->Forward() * speedDelta;
	desc->Target->Position(position);
}
