#pragma once

class MagicAttack
{
public:
	MagicAttack(class IFocus* player, MeshInstancing* model);
	~MagicAttack();

public:
	class AttackAnimation* MakeAttackInstance(Transform* parent);
	void Update();

public:
	void Attack(float value) { attack = value; }
	float Attack() { return attack; }
	void ReadyTime(float value) { readyTime = value; }
	float ReadyTime() { return readyTime; }
	void LifeTime(float value) { lifeTime = value; }
	float LifeTime() { return lifeTime; }
	void DelayTime(float value) { delayTime = value; }
	float DelayTime() { return delayTime; }
	void Tag(wstring value) { tag = value; }
	wstring Tag() { return tag; }
	Transform* InitTransform() { return initTransform; }

public:
	void FireBullet(const Vector3& point, const Quaternion& q);

private:
	float readyTime = 0.25f;
	float delayTime = 0.75f;
	float attack = 3.0f;
	Transform* initTransform;

private:
	class Instance_M;
	vector<Instance_M*> instances;

	MeshInstancing* sphere;
	class RotateSystem* rotateSystem;
	class IFocus* player;
	float lifeTime = 15.0f;
	wstring tag;

private:
	struct Bullet
	{
		float LifeTime;
		MeshInstance* Model;
		class AttackSystem* System;
		ColliderBox* HitBox;
	};
	list<Bullet> bullets;

private:
	class Instance_M
	{
	public:
		Instance_M(MagicAttack* init, Transform* parent);
		~Instance_M();

	public:
		class AttackAnimation* Anim();

	private:
		bool Update(float runningTime);
		bool IsAttackAble();
		void Stop();

	private:
		float readyTime;
		float delayTime;

	private:
		class AttackAnimation* anim;
		MagicAttack* init;
		Transform* transform;

	private:
		enum class AttackState {
			None, Ready, Delay, Stop
		} state;
	};
};
