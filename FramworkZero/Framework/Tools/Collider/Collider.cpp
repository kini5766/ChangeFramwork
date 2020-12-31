#include "Framework.h"
#include "Collider.h"

Collider::Collider(UINT instanceId)
	: instanceId(instanceId)
{
	transform = new Transform();
}

Collider::~Collider()
{
	SafeDelete(transform);
}

void Collider::Relese()
{
	CollisionManager::Get()->ReleseCollider(this);
}

void Collider::UpdateBounding()
{
	Bounding& a = bounding;

	Matrix wa;
	transform->LossyWorld(&wa);

	a.AxisX = Vector3(wa._11, wa._12, wa._13);
	a.AxisY = Vector3(wa._21, wa._22, wa._23);
	a.AxisZ = Vector3(wa._31, wa._32, wa._33);
	a.Position = Vector3(wa._41, wa._42, wa._43);
	a.HalfSize.x = D3DXVec3Length(&a.AxisX);
	a.HalfSize.y = D3DXVec3Length(&a.AxisY);
	a.HalfSize.z = D3DXVec3Length(&a.AxisZ);
	a.AxisX /= a.HalfSize.x;
	a.AxisY /= a.HalfSize.y;
	a.AxisZ /= a.HalfSize.z;
	a.HalfSize *= 0.5f;
}

bool Collider::Intersection(const Vector3 & position, const Vector3 & direction, float * outDistance)
{
	return Intersection(Ray(position, direction), outDistance);
}

bool Collider::Intersection(Ray ray, float * outDistance)
{
	Matrix w;
	transform->LossyWorld(&w);
	Vector3 scale;
	scale.x = D3DXVec3Length(&Vector3(w._11, w._12, w._13));
	scale.y = D3DXVec3Length(&Vector3(w._21, w._22, w._23));
	scale.z = D3DXVec3Length(&Vector3(w._31, w._32, w._33));
	scale *= 0.5f;

	Matrix invW;
	D3DXMatrixInverse(&invW, nullptr, &w);
	D3DXVec3TransformCoord(&ray.Position, &ray.Position, &invW);
	D3DXVec3TransformNormal(&ray.Direction, &ray.Direction, &invW);

	Vector3 point(0.0f, 0.0f, 0.0f);

	// x충돌 검사
	// 박스 안에 레이존재
	if (abs(ray.Position.x) < scale.x)
		point.x = 0.0f;
	// P | D
	// + * + : 충돌아님
	// + * - : 충돌
	// - * + : 충돌
	// - * - : 충돌아님
	else if (ray.Position.x * ray.Direction.x < 0.0f)
		point.x = abs(ray.Position.x) - scale.x;
	// 충돌아님
	else return false;

	if (abs(ray.Position.y) < scale.y)
		point.y = 0.0f;
	else if (ray.Position.y * ray.Direction.y < 0.0f)
		point.y = abs(ray.Position.y) - scale.y;
	else return false;

	if (abs(ray.Position.z) < scale.z)
		point.z = 0.0f;
	else if (ray.Position.z * ray.Direction.z < 0.0f)
		point.z = abs(ray.Position.z) - scale.z;
	else return false;

	if (outDistance != nullptr)
	{
		if (fabsf(ray.Direction.x == 0)) ray.Direction.x = 1e-6f;
		if (fabsf(ray.Direction.y == 0)) ray.Direction.y = 1e-6f;
		if (fabsf(ray.Direction.z == 0)) ray.Direction.z = 1e-6f;

		point.x /= ray.Direction.x;
		point.y /= ray.Direction.y;
		point.z /= ray.Direction.z;

		*outDistance = 0.0f;
		*outDistance = max(*outDistance, abs(point.x));
		*outDistance = max(*outDistance, abs(point.y));
		*outDistance = max(*outDistance, abs(point.z));

		// hitPoint = ray.Position + ray.Direction * (*outDistance);
	}

	return true;
}

bool Collider::Intersection(Collider * other)
{
	return Collision(this->bounding, other->bounding);
}

bool Collider::SepratePlane(const Vector3 & distance, const Vector3 & direction, const Bounding & box1, const Bounding & box2)
{
	float dotDist = fabsf(D3DXVec3Dot(&direction, &distance));
	float dotSize = 0.0f;
	dotSize += fabsf(D3DXVec3Dot(&direction, &box1.AxisX) * box1.HalfSize.x);
	dotSize += fabsf(D3DXVec3Dot(&direction, &box1.AxisY) * box1.HalfSize.y);
	dotSize += fabsf(D3DXVec3Dot(&direction, &box1.AxisZ) * box1.HalfSize.z);

	dotSize += fabsf(D3DXVec3Dot(&direction, &box2.AxisX) * box2.HalfSize.x);
	dotSize += fabsf(D3DXVec3Dot(&direction, &box2.AxisY) * box2.HalfSize.y);
	dotSize += fabsf(D3DXVec3Dot(&direction, &box2.AxisZ) * box2.HalfSize.z);

	return dotDist > dotSize;
}

bool Collider::Collision(const Bounding & box1, const Bounding & box2)
{
	Vector3 distance = box2.Position - box1.Position;

	if (SepratePlane(distance, box1.AxisX, box1, box2)) return false;
	if (SepratePlane(distance, box1.AxisY, box1, box2)) return false;
	if (SepratePlane(distance, box1.AxisZ, box1, box2)) return false;

	if (SepratePlane(distance, box2.AxisX, box1, box2)) return false;
	if (SepratePlane(distance, box2.AxisY, box1, box2)) return false;
	if (SepratePlane(distance, box2.AxisZ, box1, box2)) return false;

	if (SepratePlane(distance, Cross(box1.AxisX, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisX, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisX, box2.AxisZ), box1, box2)) return false;

	if (SepratePlane(distance, Cross(box1.AxisY, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisY, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisY, box2.AxisZ), box1, box2)) return false;

	if (SepratePlane(distance, Cross(box1.AxisZ, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisZ, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(distance, Cross(box1.AxisZ, box2.AxisZ), box1, box2)) return false;

	return true;
}

Vector3 Collider::Cross(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}
