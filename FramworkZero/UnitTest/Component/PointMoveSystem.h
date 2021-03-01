#pragma once

// 목표 지점 이동 시스탬
class PointMoveSystem
{
public:
	PointMoveSystem(Transform* transform);
	~PointMoveSystem();

public:
	void PreUpdate(const Vector3& point, float* outLengthSq = nullptr);
	bool Update();

public:
	void SetMoveSpeed(float value) { movingSpeed = value; }
	void SetRotateSpeed(float value) { rotateSpeed = value; }
	Transform* GetTransform() { return transform; }

private:
	// 움직일 대상
	Transform* transform;
	// 걷기 속도
	float movingSpeed = 0.0f;
	// 회전 속도
	float rotateSpeed = 1.0f;
	// 회전 보간 시스템
	class RotateSystem* sysRatate;

	// 목표지점
	Vector3 point = Vector3(0, 0, 0);

private:
	Vector3 destance;
	Vector3 direction;
	float lengthSq;
};