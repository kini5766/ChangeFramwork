#pragma once

struct AttackDesc
{
	float Attack = 10.0f;
};

class AttackSystem
{
public:
	AttackSystem();
	~AttackSystem();

public:
	void StartAttack();
	void EndAttack();
	bool IsAttacking();

public:
	Transform* GetTransform();
	void SetTag(wstring value);
	void SetAttack(float value) { attack.Attack = value; }

private:
	AttackDesc attack;
	SendBox* sendbox;
};
