#pragma once

struct DragPlane
{
public:
	void Set();

private:
	Vector3 StartPoint;
	Vector3 EndPoint;

	Plane* PlaneSlope;
	Vector2* Vec2Rad;

	float* Rad;
	float* Length;

public:
	DragPlane(struct BrushEditor& value);
};