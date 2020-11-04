#pragma once

class EquippableKachujin
{
public:
	EquippableKachujin();
	~EquippableKachujin();

public:
	void Update();
	void Render();

public:
	void Unarmed();
	void Armed(int num);
	void SetEquipItem(int num);
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f);

private:
	void Kachujin();
	void Sword();
	void Sword_epic();
	void Sword2();
	void Rapier();
	void LongBow();
	void Katana();
	void Dagger_small();
	void Dagger_epic();
	void Cutter2();
	void Cutter();

private:
	Shader* shader;
	ModelAttacher* kachujin = nullptr;
	bool bGetWeapon = false;
	UINT clipNum = 0;
	int equipedItemNum = 0;
	int chageItem = -1;

	enum class State
	{
		None,
		ArmedBefore,
		ArmedAfter,
		UnarmedBefore,
		UnarmedAfter,
	}state;
};