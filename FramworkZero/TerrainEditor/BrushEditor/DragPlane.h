#pragma once

struct DragPlane
{
public:
	void Set();

public:
	const Vector3& StartPoint;
	const Vector3& EndPoint;
	Plane* PlaneSlope;
	Vector2* Vec2Rad;

	float* Rad;
	float* Length;

public:
	DragPlane(struct BrushEditor& value);
};