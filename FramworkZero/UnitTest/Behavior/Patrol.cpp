#include "stdafx.h"
#include "Patrol.h"

#include "Component/RotateSystem.h"

Patrol::Patrol(const PatrolDesc & desc)
	: desc(new PatrolDesc(desc))
{
}

Patrol::~Patrol()
{
	SafeDelete(desc);
}

void Patrol::Update()
{
	idleTime -= Time::Delta();
	if (idleTime >= 0.0f)
		return;

	Vector3 position;
	desc->Target->Position(&position);
	Vector3 focus = points[curr];
	Vector3 dest = focus - position;
	dest.y = 0.0f;
	float lengthSq = D3DXVec3LengthSq(&dest);

	if (lengthSq < 1.0f)
	{
		(*desc->AnimNaxt) = 0;

		idleTime = 1.0f;
		curr = (curr + 1) % points.size();
	}
	else
	{
		(*desc->AnimNaxt) = 1;

		float speedDelta = 0.0f;
		speedDelta = (*desc->WalkSpeed) * Time::Delta();

		// ¼øÂû
		Vector3 dest2;
		D3DXVec3Normalize(&dest2, &(dest));
		desc->Ratate->SetTarget(dest2);

		Quaternion q;
		desc->Target->Rotation(&q);
		q = desc->Ratate->GetRotation(q, -desc->Target->Forward(), speedDelta * (*desc->TurnSpeed));
		desc->Target->Rotation(q);

		position += -desc->Target->Forward() * speedDelta;
		desc->Target->Position(position);
	}
}

void Patrol::SetPoints(const vector<Vector3>& values)
{
	points.assign(values.begin(), values.end());
}
