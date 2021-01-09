#pragma once

class AttackAnimation
{
public:
	AttackAnimation();
	~AttackAnimation();

public:
	void Update();
	bool IsAttackAble();
	void Play();
	void Stop();
	void Reset();

public:
	void AddUnit(function<bool(float)> funcUpdate);
	void SetFuncStop(function<void(void)> value) { funcStop = value; }
	void SetFuncAttackAble(function<bool(void)> value) { funcAttackAble = value; }

private:
	function<void(void)> funcStop;
	function<bool(void)> funcAttackAble;

private:
	struct Unit
	{
		// bool : 애니메이션 종료 여부, float : 재생시간
		function<bool(float)> Update;
		float RunningTime = 0.0f;
	};
	vector<Unit*> units;
	vector<UINT> junkUnits;
	list<UINT> playList;
};
