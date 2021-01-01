#pragma once

class Collider
{
public:
	static const UINT COLLIDER_LAYER_NONE = 0x00000000u;
	static const UINT COLLIDER_LAYER_ALL = 0xFFFFFFFFu;
	static const UINT COLLIDER_LAYER_DEFAULT = 0xFFFFFF0Fu;
	static const UINT COLLIDER_LAYER_CAMERA = 0x00000002u;
	static const UINT COLLIDER_LAYER_HITBOX = 0x00000020u;
	static const UINT COLLIDER_LAYER_ATTACKBOX = 0x00000040u;

public:
	Collider(UINT instanceId);
	~Collider();

	// Getter, Setter
public:
	Transform* GetTransform() { return initTransform; }
	bool IsActiveSelf() const { return bActive; }
	void SetActiveSelf(bool value) { bActive = value; }
	void SetLayer(UINT value) { layer = value; }

	// instance
public:
	void Release();
	UINT Id() const { return instanceId; }

public:
	void UpdateBounding();
public:
	UINT GetMask() const { return (bActive) ? layer : COLLIDER_LAYER_NONE; }
	bool Intersection(const Vector3& position, const Vector3& direction, float* outDistance) const;
	bool Intersection(const Ray& ray, float* outDistance) const;
	bool Intersection(Collider* other) const;

private:
	Transform* initTransform;
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
	bool SepratePlane(const Vector3& distance, const Vector3& direction, const Bounding& box1, const Bounding& box2) const;
	bool Collision(const Bounding& box1, const Bounding& box2) const;
	Vector3 Cross(const Vector3& v1, const Vector3& v2) const;
};