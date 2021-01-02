#pragma once

class Collider
{
public:
	Collider();
	~Collider();

	// Getter, Setter
public:
	Transform* GetTransform() { return initTransform; }

public:
	void UpdateBounding();

public:
	bool Intersection(const Vector3& position, const Vector3& direction, float* outDistance) const;
	bool Intersection(const Ray& ray, float* outDistance) const;
	bool Intersection(const Collider* other) const;


private:
	Transform* initTransform;
	struct Bounding
	{
		Vector3 Position;
		Vector3 AxisX;
		Vector3 AxisY;
		Vector3 AxisZ;
		Vector3 HalfSize;
	}bounding;

private:
	bool CheckRayPlane(Vector3* outPoint, const Vector3& ori, const Vector3& dir, UINT axis) const;
	bool SepratePlane(const Vector3& distance, const Vector3& direction, const Bounding& box1, const Bounding& box2) const;
	bool Collision(const Bounding& box1, const Bounding& box2) const;
	Vector3 Cross(const Vector3& v1, const Vector3& v2) const;
};