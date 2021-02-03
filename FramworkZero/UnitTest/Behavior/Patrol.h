#pragma once

struct PatrolDesc
{
	Transform* Target;
	class RotateSystem* Ratate;  // 회전 보간
	UINT* AnimNaxt;

	float* WalkSpeed;
	float* TurnSpeed;
};

class Patrol
{
public:
	Patrol(const PatrolDesc& desc);
	~Patrol();

public:
	void Update();

public:
	void SetPoints(const vector<Vector3>& values);

private:
	PatrolDesc* desc;
	UINT curr = 0;
	float idleTime = 0.0f;
	vector<Vector3> points;
};