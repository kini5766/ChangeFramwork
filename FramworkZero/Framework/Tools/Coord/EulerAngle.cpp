#include "Framework.h"
#include "EulerAngle.h"

EulerAngle::EulerAngle()
{
}

EulerAngle::EulerAngle(Vector3 eulerRadius)
	: euler(eulerRadius)
{
}

EulerAngle::EulerAngle(const Quaternion & q)
{
	SetQuaternion(q);
}

EulerAngle::EulerAngle(float xDeg, float yDeg, float zDeg)
{
	SetDegree(xDeg, yDeg, zDeg);
}

EulerAngle::~EulerAngle()
{
}

Vector3 EulerAngle::EulerRadius() const
{
	return euler;
}

Vector3 EulerAngle::EulerDegree() const
{
	return euler * Math::Rad2Deg;
}

void EulerAngle::ToQuaternion(Quaternion* out) const
{
	D3DXQuaternionRotationYawPitchRoll(out, euler.y, euler.x, euler.z);
}

void EulerAngle::SetRadius(const Vector3& eulerRadius)
{
	euler = eulerRadius;
}

void EulerAngle::SetDegree(const Vector3& eulerDegree)
{
	SetRadius(eulerDegree * Math::Deg2Rad);
}

void EulerAngle::SetDegree(float xDeg, float yDeg, float zDeg)
{
	SetDegree(Vector3(xDeg, yDeg, zDeg));
}

void EulerAngle::SetQuaternion(const Quaternion& q)
{
	float xy2 = 2 * q.x * q.y;
	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	euler.x = atan2(xw2 - yz2, 1 - xx2 - zz2);
	euler.y = atan2(yw2 - xz2, 1 - yy2 - zz2);
	euler.z = asin(xy2 + zw2);
}

void EulerAngle::RotateQuaternion(Quaternion * inout)
{
	Quaternion q;
	D3DXQuaternionRotationYawPitchRoll(&q, 0.0f, euler.y, 0.0f);
	(*inout) = q * (*inout);

	D3DXQuaternionRotationYawPitchRoll(&q, euler.x, 0.0f, 0.0f);
	(*inout) = q * (*inout);

	D3DXQuaternionRotationYawPitchRoll(&q, 0.0f, 0.0f, euler.z);
	(*inout) = (*inout) * q;
}
