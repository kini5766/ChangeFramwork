#pragma once

class HPSystem
{
public:
	HPSystem();
	~HPSystem();

public:
	void Update();
	void Render();

public:
	Transform* GetHpbar();
	ColliderBox* GetHurtbox() { return hurtbox; }
	void AddReceiveTag(wstring value);
	void SetFuncDamage(function<void(void)> value) { funcDamage = value; }
	float HP() { return hp; }

private:
	ColliderBox* hurtbox;
	ReceiveBox* hurtReceiver;

	float hp = 100.0f;
	float hpMax = 100.0f;
	
	class HPBar* hpBar;
	function<void(void)> funcDamage = []() {};
	
	wstring tag;
};