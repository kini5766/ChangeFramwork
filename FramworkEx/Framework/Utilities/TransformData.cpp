#include "Framework.h"
#include "TransformData.h"

// ----------------------------------------------------------------------------
// TransformData
// ----------------------------------------------------------------------------

TransformData::TransformData()
	: position(0, 0, 0), scale(1, 1, 1)
{
	D3DXQuaternionIdentity(&rotation);
}

TransformData::~TransformData()
{
}

void TransformData::Set(TransformData * value)
{
	position = value->position;
	scale = value->scale;
	rotation = value->rotation;
}

void TransformData::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void TransformData::Position(const Vector3 & value)
{
	position = value;
}

void TransformData::Position(Vector3 * out)
{
	*out = position;
}

void TransformData::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void TransformData::Scale(const Vector3 & value)
{
	scale = value;
}

void TransformData::Scale(Vector3 * out)
{
	*out = scale;
}

void TransformData::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void TransformData::RotationDegree(const Vector3 & value)
{
	Rotation(value * Math::Deg2Rad());
}

void TransformData::RotationDegree(Vector3 * out)
{
	Rotation(out);
	(*out) *= Math::Rad2Deg();
}

void TransformData::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void TransformData::Rotation(const Vector3 & value)
{
	D3DXQuaternionRotationYawPitchRoll(&rotation, value.y, value.x, value.z);
}

void TransformData::Rotation(Vector3 * out)
{
	Quaternion& q = rotation;

	float xy2 = 2 * q.x * q.y;
	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	out->x = asin(xy2 + zw2);
	out->y = atan2(yw2 - xz2, 1 - yy2 - zz2);
	out->z = atan2(xw2 - yz2, 1 - xx2 - zz2);
}

void TransformData::Rotation(const Quaternion & value)
{
	rotation = value;
}

void TransformData::Rotation(Quaternion * out)
{
	*out = rotation;
}

void TransformData::RotateYawDegree(float deg)
{
	RotateYaw(deg * Math::Deg2Rad());
}

void TransformData::RotateYaw(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, rad, 0.0f);

	rotation = euler * rotation;
}

void TransformData::RotatePitchDegree(float deg)
{
	RotatePitch(deg * Math::Deg2Rad());
}

void TransformData::RotatePitch(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, rad, 0.0f, 0.0f);

	rotation = euler * rotation;
}

void TransformData::RotateRollDegree(float deg)
{
	RotateRoll(deg * Math::Deg2Rad());
}

void TransformData::RotateRoll(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, 0.0f, rad);

	rotation *= euler;
}

Vector3 TransformData::Forward()
{
	Quaternion& q = rotation;

	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;

	return Vector3(xz2 + yw2, yz2 - xw2, 1.0f - xx2 - yy2);
}

Vector3 TransformData::Up()
{
	Quaternion& q = rotation;

	float xy2 = 2 * q.x * q.y;
	float yz2 = 2 * q.y * q.z;

	float xx2 = 2 * q.x * q.x;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float zw2 = 2 * q.z * q.w;

	return Vector3(xy2 - zw2, 1.0f - xx2 - zz2, yz2 + xw2);
}

Vector3 TransformData::Right()
{
	Quaternion& q = rotation;

	float xy2 = 2 * q.x * q.y;
	float xz2 = 2 * q.x * q.z;

	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	return Vector3(1.0f - yy2 - zz2, xy2 + zw2, xz2 - yw2);
}

void TransformData::World(const Matrix & set)
{
	D3DXMatrixDecompose(&scale, &rotation, &position, &set);  // ÄõÅÍ´Ï¿Â
}

void TransformData::World(Matrix * out)
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	(*out) = S * R * T;
}


// ----------------------------------------------------------------------------
// TransformFamily
// ----------------------------------------------------------------------------

TransformFamily::TransformFamily()
{
}

TransformFamily::~TransformFamily()
{
}

void TransformFamily::SetParent(TransformFamily * value)
{
	RemoveParent();
	if (value != nullptr)
		value->AddChild(this);
	parent = value;
}

void TransformFamily::RemoveParent()
{
	if (parent == nullptr)
		return;

	parent->RemoveChild(this);
}

bool TransformFamily::RemoveChild(TransformFamily * value)
{
	vector<TransformFamily*>::iterator iter = childs.begin();
	while (iter != childs.end())
	{
		if ((*iter) == value)
		{
			childs.erase(iter);
			return true;
		}

		++iter;
	}
	return false;
}

void TransformFamily::AddChild(TransformFamily * value)
{
	childs.push_back(value);
}
