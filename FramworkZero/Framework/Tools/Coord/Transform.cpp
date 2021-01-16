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
	globalWorld = new Matrix();
	D3DXMatrixIdentity(globalWorld);
}

Transform::Transform(Matrix * pMatrixGetter)
	: position(0, 0, 0), scale(1, 1, 1)
{
	D3DXQuaternionIdentity(&rotation);

	bMyWorld = false;
	globalWorld = pMatrixGetter;
	D3DXMatrixIdentity(globalWorld);
}

Transform::~Transform()
{
	if (bMyWorld)
		SafeDelete(globalWorld);
}


#pragma region Transform

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

void Transform::Rotation(const EulerAngle & value)
{
	value.ToQuaternion(&rotation);
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
	D3DXMatrixDecompose(&scale, &rotation, &position, &set);  // 쿼터니온
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

void Transform::GlobalWorld(const Matrix & set)
{
	Matrix invParent;
	if (parent != nullptr)
	{
		parent->GlobalWorld(&invParent);
		D3DXMatrixInverse(&invParent, nullptr, &invParent);
	}
	else D3DXMatrixIdentity(&invParent);

	LocalWorld(set * invParent);

	memcpy_s(globalWorld, sizeof(Matrix), &set, sizeof(Matrix));
	changed = false;
}

void Transform::GlobalWorld(Matrix * out)
{
	UpdateWorld();
	memcpy(out, globalWorld, sizeof(Matrix));
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

	LocalWorld(globalWorld);
	changed = false;

	if (parent == nullptr)
		return;

	Matrix p;
	parent->GlobalWorld(&p);
	(*globalWorld) *= p;
}

void Transform::ReplaceMatrixGetter(Matrix * pMatrixGetter)
{
	memcpy(pMatrixGetter, globalWorld, sizeof(Matrix));

	if (bMyWorld)
		SafeDelete(globalWorld);

	bMyWorld = false;
	globalWorld = pMatrixGetter;
}

#pragma endregion


#pragma region 	TransformFamily

void Transform::SetParent(Transform * value)
{
	if (value == this)
		return;

	Matrix lossy;
	GlobalWorld(&lossy);

	RemoveThis(parent);
	AddThis(parent, value);
	parent = value;

	GlobalWorld(lossy);
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

	// 루프로 연결 되는 것을 방지하기 위해 ('A - B - C - A' 이런 형태가 되면 무한 루프가 된다)
	for (auto p = newparent->parent; p != nullptr; p = p->parent)
	{
		// 새 부모의 부모가 자기 자신이라면 그 부모의 부모를 옛 부모로 만든다. 
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
