#include "Framework.h"
#include "Transform.h"

// ----------------------------------------------------------------------------
// Transform
// ----------------------------------------------------------------------------

Transform::Transform()
	: position(0, 0, 0), scale(1, 1, 1)
{
	D3DXQuaternionIdentity(&rotation);

	bMyWorld = true;
	lossyWorld = new Matrix();
	D3DXMatrixIdentity(lossyWorld);
}

Transform::Transform(Matrix * pMatrixGetter)
	: position(0, 0, 0), scale(1, 1, 1)
{
	D3DXQuaternionIdentity(&rotation);

	bMyWorld = false;
	lossyWorld = pMatrixGetter;
	D3DXMatrixIdentity(lossyWorld);
}

Transform::~Transform()
{
	if (bMyWorld)
		SafeDelete(lossyWorld);
}


#pragma region Transform

void Transform::Set(Transform * value)
{
	position = value->position;
	scale = value->scale;
	rotation = value->rotation;
	ChangedWorld();
}

void Transform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Transform::Position(const Vector3 & value)
{
	position = value;
	ChangedWorld();
}

void Transform::Position(Vector3 * out)
{
	*out = position;
}

void Transform::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void Transform::Scale(const Vector3 & value)
{
	scale = value;
	ChangedWorld();
}

void Transform::Scale(Vector3 * out)
{
	*out = scale;
}

void Transform::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Transform::RotationDegree(const Vector3 & value)
{
	Rotation(value * Math::Deg2Rad);
}

void Transform::RotationDegree(Vector3 * out)
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

	(*out).x = atan2(xw2 - yz2, 1 - xx2 - zz2);
	(*out).y = atan2(yw2 - xz2, 1 - yy2 - zz2);
	(*out).z = asin(xy2 + zw2);

	(*out) *= Math::Rad2Deg;
}

void Transform::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Transform::Rotation(const Vector3 & value)
{
	D3DXQuaternionRotationYawPitchRoll(&rotation, value.y, value.x, value.z);
	ChangedWorld();
}

void Transform::Rotation(const Quaternion & value)
{
	rotation = value;
	ChangedWorld();
}

void Transform::Rotation(Quaternion * out)
{
	*out = rotation;
}

void Transform::RotateYawDegree(float deg)
{
	RotateYaw(deg * Math::Deg2Rad);
}

void Transform::RotateYaw(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, rad, 0.0f);

	rotation = euler * rotation;
	ChangedWorld();
}

void Transform::RotatePitchDegree(float deg)
{
	RotatePitch(deg * Math::Deg2Rad);
}

void Transform::RotatePitch(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, rad, 0.0f, 0.0f);

	rotation = euler * rotation;
	ChangedWorld();
}

void Transform::RotateRollDegree(float deg)
{
	RotateRoll(deg * Math::Deg2Rad);
}

void Transform::RotateRoll(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, 0.0f, rad);

	rotation *= euler;
	ChangedWorld();
}

Vector3 Transform::Forward()
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

Vector3 Transform::Up()
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

Vector3 Transform::Right()
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

void Transform::LocalWorld(const Matrix & set)
{
	D3DXMatrixDecompose(&scale, &rotation, &position, &set);  // ÄõÅÍ´Ï¿Â
	ChangedWorld();
}

void Transform::LocalWorld(Matrix * out)
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationQuaternion(&R, &rotation);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	(*out) = S * R * T;
}

#pragma endregion


#pragma region 	World Matrix

void Transform::LossyWorld(const Matrix & set)
{
	Matrix invParent;
	if (parent != nullptr)
	{
		parent->LossyWorld(&invParent);
		D3DXMatrixInverse(&invParent, nullptr, &invParent);
	}
	else D3DXMatrixIdentity(&invParent);

	LocalWorld(set * invParent);

	memcpy_s(lossyWorld, sizeof(Matrix), &set, sizeof(Matrix));
	changed = false;
}

void Transform::LossyWorld(Matrix * out)
{
	UpdateWorld();
	memcpy(out, lossyWorld, sizeof(Matrix));
}

void Transform::ChangedWorld()
{
	changed = true;
	for (auto child : childs)
		child->ChangedWorld();
}

void Transform::UpdateWorld()
{
	if (changed == false)
		return;

	LocalWorld(lossyWorld);
	changed = false;

	if (parent == nullptr)
		return;

	Matrix p;
	parent->LossyWorld(&p);
	(*lossyWorld) *= p;
}

void Transform::ReplaceMatrixGetter(Matrix * pMatrixGetter)
{
	memcpy(pMatrixGetter, lossyWorld, sizeof(Matrix));

	if (bMyWorld)
		SafeDelete(lossyWorld);

	bMyWorld = false;
	lossyWorld = pMatrixGetter;
}

#pragma endregion


#pragma region 	TransformFamily

void Transform::SetParent(Transform * value)
{
	if (value == this)
		return;

	Matrix lossy;
	LossyWorld(&lossy);

	RemoveThis(parent);
	AddThis(parent, value);
	parent = value;

	LossyWorld(lossy);
}

void Transform::UnLink()
{
	for (UINT i = 0; i < childs.size(); i++)
	{
		childs[i]->SetParent(parent);
	}
	SetParent(nullptr);
}

void Transform::AddThis(Transform * oldParent, Transform * newparent)
{
	if (newparent == nullptr) return;

	for (auto p = newparent->parent; p != nullptr; p = p->parent)
	{
		if (p->parent == this)
		{
			p->SetParent(oldParent);
			break;
		}
	}
	newparent->childs.push_back(this);
}

void Transform::RemoveThis(Transform * oldParent)
{
	if (oldParent == nullptr) return;

	vector<Transform*>::iterator iter = oldParent->childs.begin();
	while (iter != oldParent->childs.end())
	{
		if ((*iter) == this)
		{
			oldParent->childs.erase(iter);
			return;
		}

		++iter;
	}
}

#pragma endregion
