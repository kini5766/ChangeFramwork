#pragma once

struct MagicBulletInput
{
	class IFocus* Focus = nullptr;
	float Attack;
	wstring Tag;

	Vector3 Point;
	Quaternion Rotation;
};

class MagicMaker
{
public:
	MagicMaker(MeshInstancing* model);
	~MagicMaker();

public:
	void Update();

public:
	void AddBullet(const MagicBulletInput& input);


public:
	void LifeTime(float value) { lifeTime = value; }
	float LifeTime() { return lifeTime; }
	void Tag(wstring value) { tag = value; }
	wstring Tag() { return tag; }
	Transform* InitTransform() { return initTransform; }

private:
	MeshInstancing* model;
	Transform* initTransform;
	class RotateSystem* rotateSystem;
	float lifeTime = 15.0f;
	wstring tag;

private:
	struct Bullet
	{
		float LifeTime;
		MeshInstance* Model;
		class AttackSystem* System;
		ColliderBox* HitBox;

		class IFocus* Focus;
	};
	list<Bullet> bullets;
};