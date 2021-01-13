#pragma once

class RotateSystem
{
public:
	RotateSystem();
	~RotateSystem();

public:
	void SetTarget(const Vector3& value) { target = value; }

public:
	Quaternion GetRotation(const Quaternion& q, const Vector3& forward, float speedDelta, float* outRad = nullptr);

private:
	Vector3 target;
};