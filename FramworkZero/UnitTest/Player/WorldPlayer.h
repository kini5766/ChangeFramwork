#pragma once

#include "Rendering/Camera/Main/IFocus.h"

class WorldPlayer
{
public:
	WorldPlayer();
	~WorldPlayer();

public:
	void Update();
	void Render();
	void PreRender();
	void PostRender();

	IFocus* GetFocus();

private:
	void Player(ModelInstance* instance);
	void PlayerHp(Transform* transform);
	void PlayerAttack();
	void PlayerWeapon(Transform* transform);

private:
	void OnDamage();

private:
	ModelInstancing* kachujinMaker;
	class CharacterController* player;
	class KachujinInstance* kachujin;
	class NormalAttack* attack;
	class HPSystem* playerHp;

	MeshInstancing* mesh;
	Transform* weapon;
	bool bLost = false;

	bool bStealth = true;
	EnvCubeMap* envCubeMap;
};