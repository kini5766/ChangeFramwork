#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;
	virtual void ResizeScreen(float width, float height) = 0;

public:
	Perspective* GetPerspective() { return (Perspective*)projection; }
	Projection* GetProjection() { return projection; }
	Viewport* GetViewport() { return viewport; }

public:
	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(const Vector3& value);

	void Rotation(float x, float y, float z);
	void Rotation(const Vector3& value);

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	void RotateYawDegree(float deg);
	void RotateYaw(float rad);

	void RotatePitchDegree(float deg);
	void RotatePitch(float rad);

	void RotateRollDegree(float deg);
	void RotateRoll(float rad);

	void GetMatrix(Matrix* matrix);

	Vector3 Forward() { return forward; }
	Vector3 Up() { return up; }
	Vector3 Right() { return right; }

protected:
	virtual void Rotation();
	virtual void Move();

protected:
	void View();

protected:
	Projection* projection;
	Viewport* viewport;

private:
	Vector3 position = Vector3(0, 0, 0);
	Quaternion rotation = Quaternion(0, 0, 0, 1);

	Vector3 forward = Vector3(0, 0, 1);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = Vector3(1, 0, 0);

	Matrix matView;
};