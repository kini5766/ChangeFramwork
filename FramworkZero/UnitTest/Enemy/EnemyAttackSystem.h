#pragma once


class EnemyAttackSystem
{
public:
	EnemyAttackSystem();
	~EnemyAttackSystem();

public:
	void Update();
	bool IsAttackAble();
	void OnAttack();

public:
	Transform* GetTransform();
	void SetTag(wstring value);

public:
	void ReadyTime(float value) { readyTime = value; }
	void EndTime(float value) { endTime = value; }
	void DelayTime(float value) { delayTime = value; }

private:
	float readyTime = 0.5f;
	float endTime = 1.05f;
	float delayTime = 2.0f;
	float runningTime = 0.0f;
	class AttackSystem* system;

	enum class AttackState{ 
		None, Ready, Attack, Delay
	} state;
};
