#pragma once

// 목표 지점 이동 시스탬
class PointMoveSystem
{
public:
	PointMoveSystem(Transform* transform);
	~PointMoveSystem();

public:
	bool GoToPoint();

public:
	// 목표지점으로부터 남은 거리
	void SetPoint(const Vector3& value) { point = value; }
	void SetMoveSpeeder(float* value) { speeder = value; }
	void SetRotateSpeed(float value) { rotateSpeed = value; }
	void SetSafeRangeSq(float value) { safeRangeSq = value; }

	float GetLengthSq();
	Transform* GetTransform() { return transform; }

private:
	// 움직일 대상
	Transform* transform;
	// 이동 속도
	float* speeder;
	// 회전 속도
	float rotateSpeed = 1.0f;
	// 회전 보간 시스템
	class RotateSystem* sysRatate;

	// 목표지점
	Vector3 point = Vector3(0, 0, 0);
	// 목표지점의 허용 범위 (제곱)
	float safeRangeSq = 1.0f;

};
