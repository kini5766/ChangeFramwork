#pragma once

class EnemyInstancingImGui
{
public:
	EnemyInstancingImGui(class EnemyInstancingDesc* desc);
	~EnemyInstancingImGui();

public:
	void ImGuiRender();
	void Select(int i);

private:
	class EnemyInstancingDesc* desc;
	int selected = -1;
	const char* enemyTypes = {
		"Melee\0"
		"Magician\0"
	};
};