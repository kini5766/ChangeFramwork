#pragma once

// ��ǥ ���� �̵� �ý���
class PointMoveSystem
{
public:
	PointMoveSystem(Transform* transform);
	~PointMoveSystem();

public:
	bool GoToPoint();

public:
	// ��ǥ�������κ��� ���� �Ÿ�
	void SetPoint(const Vector3& value) { point = value; }
	void SetMoveSpeeder(float* value) { speeder = value; }
	void SetRotateSpeed(float value) { rotateSpeed = value; }
	void SetSafeRangeSq(float value) { safeRangeSq = value; }

	float GetLengthSq();
	Transform* GetTransform() { return transform; }

private:
	// ������ ���
	Transform* transform;
	// �̵� �ӵ�
	float* speeder;
	// ȸ�� �ӵ�
	float rotateSpeed = 1.0f;
	// ȸ�� ���� �ý���
	class RotateSystem* sysRatate;

	// ��ǥ����
	Vector3 point = Vector3(0, 0, 0);
	// ��ǥ������ ��� ���� (����)
	float safeRangeSq = 1.0f;

};
