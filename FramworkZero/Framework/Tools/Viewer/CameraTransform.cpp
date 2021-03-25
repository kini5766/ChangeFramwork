#include "Framework.h"
#include "CameraTransform.h"

CameraTransform::CameraTransform()
{
	D3DXMatrixIdentity(&matView);
}

CameraTransform::~CameraTransform()
{
}

void CameraTransform::GetView(Matrix * matrix)
{
	memcpy(matrix, matView, sizeof(Matrix));
}

void CameraTransform::SetView(CameraTransform * transform)
{
	position = transform->position;
	rotation = transform->rotation;
	Rotation();
}

void CameraTransform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void CameraTransform::Position(const Vector3 & value)
{
	position = value;

	Move();
}

void CameraTransform::Position(Vector3 * out)
{
	*out = position;
}

void CameraTransform::RotationEuler(const EulerAngle & value)
{
	rotation = value.ToQuaternion();
	Rotation();
}

EulerAngle CameraTransform::RotationEuler()
{
	return EulerAngle(rotation);
}

void CameraTransform::Rotation(const Quaternion & value)
{
	rotation = value;
	Rotation();
}

void CameraTransform::Rotation(Quaternion * out)
{
	*out = rotation;
}

void CameraTransform::Rotation()
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
	up = Vector3(xy2 - zw2, 1.0f - xx2 - zz2, yz2 + xw2);
	right = Vector3(1.0f - yy2 - zz2, xy2 + zw2, xz2 - yw2);

	UpdateView();
}

void CameraTransform::Move()
{
	UpdateView();
}

void CameraTransform::UpdateView()
{
	Vector3 l_value(position + forward);
	D3DXMatrixLookAtLH(&matView, &position, &l_value, &up);
}
