#pragma once

class CameraTransform
{
public:
	CameraTransform();
	~CameraTransform();

public:
	void GetView(Matrix* matrix);
	void SetView(CameraTransform* transform);

public:
	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void RotationEuler(const EulerAngle& value);
	EulerAngle RotationEuler();

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	Vector3 Forward() { return forward; }
	Vector3 Up() { return up; }
	Vector3 Right() { return right; }

private:
	void Rotation();
	void Move();
	void UpdateView();

private:
	Vector3 position = Vector3(0, 0, 0);
	Quaternion rotation = Quaternion(0, 0, 0, 1);

	Vector3 forward = Vector3(0, 0, 1);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = Vector3(1, 0, 0);

	Matrix matView;
};