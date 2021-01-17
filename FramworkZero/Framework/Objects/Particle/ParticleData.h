#pragma once

#include "Framework.h"

class ParticleData
{
public:
	ParticleData() {}
	~ParticleData() {}

	enum class BlendType
	{
		Opaque = 0,
		Additive,
		Alphablend
	}Type = BlendType::Opaque;

	bool IsLoop = false;

	wstring TextureFile = L"";

	UINT MaxParticles = 100;

	// ���� ��� �ӵ�
	float ReadyTime = 1.0f;
	// ���� ���� ��� �ӵ�
	float ReadyRandomTime = 0.0f;

public: // MinMax : lerp(Min, Max, t)
	// ���
	float StartVelocity = 1.0f;
	float EndVelocity = 1.0f;

	float MinHorizontalVelocity = 0.0f;
	float MaxHorizontalVelocity = 0.0f;

	float MinVerticalVelocity = 0.0f;
	float MaxVerticalVelocity = 0.0f;

	Vector3 Gravity = Vector3(0.0f, 0.0f, 0.0f);

	Color MinColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color MaxColor = Color(1.0f, 1.0f, 1.0f, 1.0f);

	float MinRotateSpeed = 0.0f;  // z Rotation
	float MaxRotateSpeed = 0.0f;  // zRotation
	
	int MinStartSize = 100;  // z Scale
	int MaxStartSize = 100;  // z Scale

	int MinEndSize = 100;  // z Scale
	int MaxEndSize = 100;  // z Scale
};