#pragma once

class Frustum
{
public:
	Frustum();
	~Frustum();

public:
	void Update(const Matrix& V, const Matrix& P);
	void Planes(Plane* planes, UINT size = 4);

public:
	bool CheckPoint(Vector3& position);
	bool CheckCube(Vector3& center, Vector3& size);
	bool CheckCube(Vector3& center, float radius);

private:
	Plane planes[6];
};