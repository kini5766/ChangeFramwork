#include "Framework.h"
#include "WorldMatrix.h"

WorldMatrix::WorldMatrix()
{
	D3DXMatrixIdentity(&world);
}

WorldMatrix::WorldMatrix(Matrix init)
	: world(init)
{
}

WorldMatrix::~WorldMatrix()
{
}


void WorldMatrix::SetWorld_Global(Transform * t)
{
	t->GlobalWorld(&world);
}

void WorldMatrix::SetWorld_Local(Transform * t)
{
	t->LocalWorld(&world);
}

void WorldMatrix::SetWorld(const Matrix & value)
{
	memcpy(&world, &value, sizeof(Matrix));
}


void WorldMatrix::GetWorld_Global(Transform * t)
{
	t->GlobalWorld(world);
}

void WorldMatrix::GetWorld_Local(Transform * t)
{
	t->LocalWorld(world);
}

void WorldMatrix::GetWorld(Matrix * out)
{
	memcpy(&out, &world, sizeof(Matrix));
}


void WorldMatrix::Position(const Vector3 & value)
{
	world._41 = value.x; world._42 = value.y; world._43 = value.z;
}

void WorldMatrix::Rotation(const Quaternion& q)
{
	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;
	float xy2 = 2 * q.x * q.y;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	Vector3 s = LossyScale();

	world._11 = (1.0f - yy2 - zz2) * s.x; world._12 = (xy2 + zw2) * s.x; world._13 = (xz2 - yw2) * s.x;
	world._21 = (xy2 - zw2) * s.y; world._22 = (1.0f - xx2 - zz2) * s.y; world._23 = (yz2 + xw2) * s.y;
	world._31 = (xz2 + yw2) * s.z; world._32 = (yz2 - xw2) * s.z; world._33 = (1.0f - xx2 - yy2) * s.z;
}


Vector3 WorldMatrix::Position()
{
	return Vector3(world._41, world._42, world._43);
}

//Quaternion WorldMatrix::Rotation()
//{
//	Quaternion out;
//	D3DXQuaternionRotationMatrix(&out, &world);
//	D3DXQuaternionNormalize(&out, &out);
//	return out;
//}

Vector3 WorldMatrix::LossyScale()
{
	return Vector3(
		D3DXVec3Length(&Vector3(world._31, world._32, world._33)),
		D3DXVec3Length(&Vector3(world._21, world._22, world._23)),
		D3DXVec3Length(&Vector3(world._11, world._12, world._13))
	);
}


Vector3 WorldMatrix::GlobalForward()
{
	Vector3 result;
	D3DXVec3Normalize(&result, &Vector3(world._31, world._32, world._33));
	return result;
}

Vector3 WorldMatrix::GlobalUp()
{
	Vector3 result;
	D3DXVec3Normalize(&result, &Vector3(world._21, world._22, world._23));
	return result;
}

Vector3 WorldMatrix::GlobalRight()
{
	Vector3 result;
	D3DXVec3Normalize(&result, &Vector3(world._11, world._12, world._13));
	return result;
}
