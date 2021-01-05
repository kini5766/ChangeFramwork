#pragma once

#include "Tools/Viewer/IFocus.h"

struct AttackDesc
{
	float Attack = 10.0f;
};

class EnemyAttackSystem
{
public:
	EnemyAttackSystem(IFocus* player);
	~EnemyAttackSystem();

public:
	Transform* GetTransform();
	void SetTag(wstring value);
	void Range(float value) { range = value; }

private:
	void StartAttack();
	void EndAttack();

private:
	AttackDesc attack;
	SendBox* sendbox;


public:
	void Update();
	bool IsAttackAble(const Vector3& position);
	void OnAttack();

private:
	float readyTime = 0.3f;
	float endTime = 1.0f;
	float delayTime = 2.0f;
	float runningTime = 0.0f;
	IFocus* player;

	float range = 5.0f;

	enum class AttackState{ 
		None, Ready, Attack, Delay
	} state;
};
