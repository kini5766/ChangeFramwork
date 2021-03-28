#pragma once

class RotateSystem
{
public:
	RotateSystem();
	~RotateSystem();

public:
	void SetDirection(const Vector3& value) { direction = value; }

public:
	Quaternion GetRotation(const Quaternion& q, const Vector3& forward, float speedDelta, float* outRad = nullptr);

private:
	Vector3 direction;
};