#pragma once

// ¹ÝÁ÷¼±
struct Ray
{
	Ray() {};
	Ray(const Vector3& position, const Vector3& direction)
		: Position(position), Direction(direction) {};

	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Direction = Vector3(0, 0, 0);
};

class Collider
{
public:
	static const UINT COLLIDER_LAYER_NONE = 0x00000000u;
	static const UINT COLLIDER_LAYER_ALL = 0xFFFFFFFFu;
	static const UINT COLLIDER_LAYER_DEFAULT = 0x00000001u;
	static const UINT COLLIDER_LAYER_CAMERA = 0x00000010u;

public:
	Collider(UINT instanceId);
	~Collider();

	// Getter, Setter
public:
	Transform* GetTransform() { return transform; }
	bool IsActiveSelf() const { return bActive; }
	void SetActiveSelf(bool value) { bActive = value; }
	void SetLayer(UINT value) { layer = value; }

	// instance
public:
	void Relese();
	UINT Id() const { return instanceId; }

public:
	void UpdateBounding();
public:
	UINT GetMask() const { return (bActive) ? layer : COLLIDER_LAYER_NONE; }
	bool Intersection(const Vector3& position, const Vector3& direction, float* outDistance);
	bool Intersection(Ray ray, float* outDistance);
	bool Intersection(Collider* other);

private:
	Transform* transform;
	bool bActive = false;
	UINT layer = 0;
	UINT instanceId;

private:
	struct Bounding
	{
		Vector3 Position;
		Vector3 AxisX;
		Vector3 AxisY;
		Vector3 AxisZ;
		Vector3 HalfSize;
	}bounding;

private:
	bool SepratePlane(const Vector3& distance, const Vector3& direction, const Bounding& box1, const Bounding& box2);
	bool Collision(const Bounding& box1, const Bounding& box2);
	Vector3 Cross(const Vector3& v1, const Vector3& v2);
};