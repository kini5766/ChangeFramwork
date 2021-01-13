#pragma once

#include "Tools/Viewer/IFocus.h"

class WorldPlayer
{
public:
	WorldPlayer(Shader* shader);
	~WorldPlayer();

public:
	void Update();
	void Render();

	IFocus* GetFocus();

private:
	void Player(ModelSkinnedInstance* instance);
	void PlayerHp(Transform* transform);
	void PlayerAttack();
	void PlayerWeapon(Shader* shader, Transform* transform);

private:
	void OnDamage();

private:
	ModelSkinnedInstancing* kachujinMaker;
	class CharacterController* player;
	class KachujinInstance* kachujin;
	class NormalAttack* attack;
	class HPSystem* playerHp;

	MeshInstancing* mesh;
	Transform* weapon;
	bool bLost = false;
};