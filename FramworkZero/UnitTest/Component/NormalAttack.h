#pragma once

// --
// NormalAttack
// --

class NormalAttack
{
public:
	NormalAttack();
	~NormalAttack();

public:
	class AttackAnimation* MakeInstance(Transform* parent);
	void Update();

public:
	void ReadyTime(float value) { readyTime = value; }
	float ReadyTime() { return readyTime; }
	void EndTime(float value) { endTime = value; }
	float EndTime() { return endTime; }
	void DelayTime(float value) { delayTime = value; }
	float DelayTime() { return delayTime; }
	void Tag(wstring value) { tag = value; }
	wstring Tag() { return tag; }
	Transform* InitTransform() { return initTransform; }

private:
	wstring tag;
	float endTime = 1.05f;
	float delayTime = 2.0f;
	float readyTime = 0.5f;
	Transform* initTransform;

	class Instance_N;
	vector<Instance_N*> instances;

private:
	class Instance_N
	{
	public:
		Instance_N(NormalAttack* init, Transform* parent);
		~Instance_N();

	public:
		class AttackAnimation* Anim();

	private:
		bool Update(float runningTime);
		bool IsAttackAble();
		void Stop();

	private:
		float endTime;
		float readyTime;
		float delayTime;

	private:
		class AttackAnimation* anim;
		class AttackSystem* system;
		enum class AttackState {
			None, Ready, Attack, Delay, Stop
		} state;
	};

};