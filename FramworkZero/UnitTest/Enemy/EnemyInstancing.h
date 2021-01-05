#pragma once


class EnemyInstancing
{
public:
	EnemyInstancing(Shader* shader, class IFocus* player);
	~EnemyInstancing();

public:
	void Update();
	void Render();

private:
	ModelSkinnedInstancing* modelInstancing;
	class KachujinInstance* character;
	class HPSystem* hp;
	class EnemyAttackSystem* attack;
};