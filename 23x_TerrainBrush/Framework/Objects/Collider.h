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
	// 바운딩 박스
	void Render(Color color);

private:
	void SetObb();
	// 축 1개의 대한 충돌 검사
	bool SepratePlane(const Vector3& position, const Vector3& direction, Obb& box1, Obb& box2);
	// 외적
	Vector3 Cross(const Vector3& v1, const Vector3& v2);

private:
	Transform* init;  // offset (보정 값)
	Transform* transform;  // 대상 원래 위치

	Vector3 lines[8];

	// private member struct
private:
	// 1. 중점과의 거리
	// 2. 내적
	// 3. 절반 합
	struct Obb
	{
		Vector3 Position;
		Vector3 AxisX;
		Vector3 AxisY;
		Vector3 AxisZ;

		Vector3 HalfSize;
	}obb;
};

