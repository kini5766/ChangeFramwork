#pragma once

class WorldMatrix
{
public:
	WorldMatrix();
	WorldMatrix(Matrix init);
	~WorldMatrix();

public:
	void SetWorld_Global(Transform* t);
	void SetWorld_Local(Transform* t);
	void SetWorld(const Matrix& value);

	void GetWorld_Global(Transform* t);
	void GetWorld_Local(Transform* t);
	void GetWorld(Matrix* out);

public:
	void Position(const Vector3& value);
	void Rotation(const Quaternion& value);

	Vector3 Position();
	Vector3 LossyScale();

	Vector3 GlobalForward();
	Vector3 GlobalUp();
	Vector3 GlobalRight();

private:
	Matrix world;
};