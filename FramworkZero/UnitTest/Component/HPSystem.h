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
	Transform* GetHpbar() { return hpbarPos; }
	ColliderBox* GetHurtbox() { return hurtbox; }

private:
	Transform* hpbarPos;
	Render2D* hpbar;
	Texture* green;
	ColliderBox* hurtbox;
	ReceiveBox* hurtReceiver;
};