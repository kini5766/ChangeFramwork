#pragma once

class HPSystem
{
public:
	HPSystem(float hpMax = 100.0f);
	~HPSystem();

public:
	void Update();
	void PostRender();

public:
	Transform* GetHpbar();
	ColliderBox* GetHurtbox() { return hurtbox; }
	void AddReceiveTag(wstring value);
	void SetFuncDamage(function<void(void)> value) { funcDamage = value; }
	float HP() { return hp; }
	void RecoveryPer(float per);

private:
	ColliderBox* hurtbox;
	ReceiveBox* hurtReceiver;

	float hp;
	float hpMax;
	
	class HPBar* hpBar;
	function<void(void)> funcDamage = []() {};
	
	wstring tag;
};