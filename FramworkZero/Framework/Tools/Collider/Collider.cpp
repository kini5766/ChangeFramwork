#include "Framework.h"
#include "Collider.h"

Collider::Collider()
{
	initTransform = new Transform();
}

Collider::~Collider()
{
	SafeDelete(initTransform);
}

void Collider::UpdateBounding()
{
	Bounding& a = bounding;

	Matrix wa;
	initTransform->LossyWorld(&wa);

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

bool Collider::Intersection(const Vector3 & position, const Vector3 & direction, float * outDistance) const
{
	return Intersection(Ray(position, direction), outDistance);
}

bool Collider::Intersection(const Ray& ray, float * outDistance) const
{
	Matrix w;
	Vector3 ori, dir;
	{
		initTransform->LossyWorld(&w);

		Matrix invW;
		D3DXMatrixInverse(&invW, nullptr, &w);
		D3DXVec3TransformCoord(&ori, &ray.Position, &invW);
		D3DXVec3TransformNormal(&dir, &ray.Direction, &invW);
	}

	Vector3 point(0.0f, 0.0f, 0.0f);
	if (CheckRayPlane(&point, ori, dir, 0) == false)
		return false;

	if (outDistance != nullptr)
	{
		D3DXVec3TransformNormal(&point, &(point), &w);
		*outDistance = D3DXVec3Length(&point);
	}

	return true;
}

bool Collider::Intersection(const Collider * other) const
{
	return Collision(this->bounding, other->bounding);
}

bool Collider::CheckRayPlane(Vector3* outPoint, const Vector3 & ori, const Vector3 & dir, UINT axis) const
{
	if (axis == 3)
	{
		// 시작 지점이 상자 안에 있음
		if (*outPoint == Vector3(0.0f, 0.0f, 0.0f))
			return true;

		// 3면 모두 바깥으로 나감
		else return false;
	}

	float oriAxis = ori[axis];
	float dirAxis = dir[axis];
	UINT plane1 = (axis + 1) % 3;
	UINT plane2 = (axis + 2) % 3;

	// 0.5f : 반직선을 역트랜스폼 이랑 곱함
	float scale = 0.5f;

	// 가까운 면 구하기 (상자는 중점에 있음)
	if (oriAxis < 0.0f) 
	{
		oriAxis = -oriAxis;
		dirAxis = -dirAxis; 
	}

	// (시작 지점이 면 안에 있으면) { 다른 면 판단 }
	if (oriAxis < scale)
		return CheckRayPlane(outPoint, ori, dir, axis + 1);

	// (반직선이 다른 방향으로 나가면) { 충돌 아님 }
	if (dirAxis >= 0.0f)
		return false;

	// 판단 면과의 거리를 구함
	*outPoint = dir * ((scale - oriAxis) / dirAxis);

	// (반 직선이 면 바깥으로 나가면) { 다른 면 판단 }
	if (fabsf((*outPoint)[plane1] + ori[plane1]) > scale ||
		fabsf((*outPoint)[plane2] + ori[plane2]) > scale)
	{
		return CheckRayPlane(outPoint, ori, dir, axis + 1);
	}

	// 반 직선이 면 안으로 통과함 { 충돌됨 }
	return true;
}

bool Collider::SepratePlane(const Vector3 & distance, const Vector3 & direction, const Bounding & box1, const Bounding & box2) const
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

bool Collider::Collision(const Bounding & box1, const Bounding & box2) const
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

Vector3 Collider::Cross(const Vector3 & v1, const Vector3 & v2) const
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}
