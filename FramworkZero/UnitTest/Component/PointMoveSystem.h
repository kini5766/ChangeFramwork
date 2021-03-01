#pragma once

// ��ǥ ���� �̵� �ý���
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
	// ������ ���
	Transform* transform;
	// �ȱ� �ӵ�
	float movingSpeed = 0.0f;
	// ȸ�� �ӵ�
	float rotateSpeed = 1.0f;
	// ȸ�� ���� �ý���
	class RotateSystem* sysRatate;

	// ��ǥ����
	Vector3 point = Vector3(0, 0, 0);

private:
	Vector3 destance;
	Vector3 direction;
	float lengthSq;
};