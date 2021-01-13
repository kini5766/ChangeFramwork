#pragma once

class AttackAnimation
{
public:
	AttackAnimation();
	~AttackAnimation();

public:
	void Update();
	bool IsAttackAble();
	bool IsAttacking();
	void Play();
	void Stop();
	void Reset();

public:
	void AddUnit(function<bool(float)> funcUpdate);
	void SetFuncStop(function<void(void)> value) { funcStop = value; }
	void SetFuncAttackAble(function<bool(void)> value) { funcAttackAble = value; }
	void SetFuncIsAttacking(function<bool(void)> value) { funcIsAttacking = value; }

private:
	function<void(void)> funcStop;
	function<bool(void)> funcAttackAble;
	function<bool(void)> funcIsAttacking;

private:
	struct Unit
	{
		// bool : �ִϸ��̼� ���� ����, float : ����ð�
		function<bool(float)> Update;
		float RunningTime = 0.0f;
	};
	vector<Unit*> units;
	vector<UINT> junkUnits;
	list<UINT> playList;
};
