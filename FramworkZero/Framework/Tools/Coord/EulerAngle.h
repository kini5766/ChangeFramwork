#pragma once

class EulerAngle
{
public:
	EulerAngle();
	EulerAngle(Vector3 eulerRadius);
	EulerAngle(const Quaternion& q);
	EulerAngle(float xDeg, float yDeg, float zDeg);
	~EulerAngle();

public:
	Vector3 EulerRadius() const;
	Vector3 EulerDegree() const;
	void ToQuaternion(Quaternion* out) const;

public:
	void SetRadius(const Vector3& eulerRadius);
	void SetDegree(const Vector3& eulerDegree);
	void SetDegree(float xDeg, float yDeg, float zDeg);
	void SetQuaternion(const Quaternion& q);

public:
	void RotateQuaternion(Quaternion* inout);

private:
	Vector3 euler = Vector3(0, 0, 0);
};