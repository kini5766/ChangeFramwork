#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&matView);

	Rotation();
	Move();
}

Camera::~Camera()
{
}

void Camera::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Camera::Position(const Vector3 & value)
{
	position = value;

	Move();
}

void Camera::Position(Vector3 * out)
{
	*out = position;
}

void Camera::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Camera::RotationDegree(const Vector3 & value)
{
	Rotation(Math::Deg2Rad * value);
}

void Camera::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Camera::Rotation(const Vector3 & value)
{
	D3DXQuaternionRotationYawPitchRoll(&rotation, value.y, value.x, value.z);
	Rotation();
}

void Camera::Rotation(const Quaternion & value)
{
	rotation = value;
	Rotation();
}

void Camera::Rotation(Quaternion * out)
{
	*out = rotation;
}

void Camera::RotateYawDegree(float deg)
{
	RotateYaw(deg * Math::Deg2Rad);
}

void Camera::RotateYaw(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, rad, 0.0f, 0.0f);

	rotation *= euler;
	Rotation();
}

void Camera::RotatePitchDegree(float deg)
{
	RotatePitch(deg * Math::Deg2Rad);
}

void Camera::RotatePitch(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, rad, 0.0f);

	rotation = euler * rotation;
	Rotation();
}

void Camera::RotateRollDegree(float deg)
{
	RotateRoll(deg * Math::Deg2Rad);
}

void Camera::RotateRoll(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, 0.0f, rad);

	rotation = euler * rotation;
	Rotation();
}

void Camera::GetMatrix(Matrix * matrix)
{
	memcpy(matrix, matView, sizeof(Matrix));
}

void Camera::Rotation()
{
	Quaternion& q = rotation;

	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;
	float xy2 = 2 * q.x * q.y;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	forward = Vector3(xz2 + yw2, yz2 - xw2, 1.0f - xx2 - yy2);
	up =	  Vector3(xy2 - zw2, 1.0f - xx2 - zz2, yz2 + xw2);
	right =	  Vector3(1.0f - yy2 - zz2, xy2 + zw2, xz2 - yw2);

	View();
}

void Camera::Move()
{
	View();
}

void Camera::View()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}
