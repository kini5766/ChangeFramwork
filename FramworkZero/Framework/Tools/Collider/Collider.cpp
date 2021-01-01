#include "Framework.h"
#include "Collider.h"

Collider::Collider(UINT instanceId)
	: instanceId(instanceId)
{
	initTransform = new Transform();
}

Collider::~Collider()
{
	SafeDelete(initTransform);
}

void Collider::Relese()
{
	CollisionManager::Get()->ReleseCollider(this);
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

		if (ori.x < 0.0f) { ori.x = -ori.x; dir.x = -dir.x; }
		if (ori.y < 0.0f) { ori.y = -ori.y; dir.y = -dir.y; }
		if (ori.z < 0.0f) { ori.z = -ori.z; dir.z = -dir.z; }
	}

	Vector3 point(0.0f, 0.0f, 0.0f);

	if (ori.z <= 0.5f)
	{
		if (ori.y <= 0.5f)
		{
			if (ori.x <= 0.5f)
			{
			}
			else if (dir.x >= 0.0f)
			{
				return false;
			}
			else
			{
				point = dir * ((0.5f - ori.x) / dir.x);
				if (fabsf(point.y + ori.y) > 0.5f || fabsf(point.z + + ori.z) > 0.5f)
					return false;
			}  // end x
		}
		else if (dir.y >= 0.0f)
		{
			return false;
		}
		else
		{
			point = dir * ((0.5f - ori.y) / dir.y);
			if (fabsf(point.x + ori.x) > 0.5f || fabsf(point.z + ori.z) > 0.5f)
				return false;
		}  // end y
	}
	else if (dir.z >= 0.0f)
	{
		return false;
	}
	else
	{
		point = dir * ((0.5f - ori.z) / dir.z);
		if (fabsf(point.x + ori.x) > 0.5f || fabsf(point.y + ori.y) > 0.5f)
			return false;
	} // end z

	if (outDistance != nullptr)
	{
		D3DXVec3TransformNormal(&point, &(point), &w);
		*outDistance = D3DXVec3Length(&point);
	}

	return true;
}

bool Collider::Intersection(Collider * other) const
{
	return Collision(this->bounding, other->bounding);
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
