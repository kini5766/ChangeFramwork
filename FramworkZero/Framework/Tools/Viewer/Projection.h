#pragma once

class Projection
{
public:
	Projection(float width, float height, float zn, float zf, float fov);
	virtual ~Projection();

	virtual void GetMatrix(Matrix* matrix);
	virtual void Resize(float width, float height);

protected:
	virtual void Set(float width, float height, float zn, float zf, float fov);

	Projection();

protected:
	float width, height;
	float zn, zf;
	float fov;

	Matrix matrix;
};