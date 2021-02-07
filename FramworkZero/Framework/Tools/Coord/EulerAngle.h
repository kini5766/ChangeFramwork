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
	static EulerAngle Degree(float xDeg, float yDeg, float zDeg);
	static EulerAngle Degree(const Vector3& eulerDegree);


public:
	Vector3 EulerRadius() const;
	Vector3 EulerDegree() const;
	Quaternion ToQuaternion() const;

public:
	void SetRadius(const Vector3& eulerRadius);
	void SetDegree(const Vector3& eulerDegree);
	void SetDegree(float xDeg, float yDeg, float zDeg);
	void SetQuaternion(const Quaternion& q);

public:
	// 이 오일러 각도 만큼 쿼터니온을 회전
	void RotateQuaternion(Quaternion* inout);

private:
	Vector3 euler = Vector3(0, 0, 0);
};