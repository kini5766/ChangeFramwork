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
	ModelSkinnedInstancing* kachujinMaker;
	class CharacterController* player;
	class KachujinInstance* kachujin;
	class NormalAttack* attack;
	class HPSystem* playerHp;

	MeshInstancing* mesh;
	Transform* weapon;
};