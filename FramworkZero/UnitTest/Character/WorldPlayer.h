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
	ModelSkinnedInstancing* kachujin;
	class KachujinInstance* character;
	class CharacterController* player;
	class HPSystem* playerHp;
};