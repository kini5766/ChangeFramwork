#pragma once

class Collider
{
	// struct
private:
	struct Obb;


	// static
public:
	bool Collision(Obb& box1, Obb& box2);


	// member
public:
	Collider(Transform* transform, Transform* offset = nullptr);
	~Collider();

public:
	bool IsIntersect(Collider* other);

	void Update();
	// �ٿ�� �ڽ�
	void Render(Color color);

private:
	void SetObb();
	// �� 1���� ���� �浹 �˻�
	bool SepratePlane(const Vector3& position, const Vector3& direction, Obb& box1, Obb& box2);
	// ����
	Vector3 Cross(const Vector3& v1, const Vector3& v2);

private:
	Transform* init;  // offset (���� ��)
	Transform* transform;  // ��� ���� ��ġ

	Vector3 lines[8];

	// private member struct
private:
	// 1. �������� �Ÿ�
	// 2. ����
	// 3. ���� ��
	struct Obb
	{
		Vector3 Position;
		Vector3 AxisX;
		Vector3 AxisY;
		Vector3 AxisZ;

		Vector3 HalfSize;
	}obb;
};

