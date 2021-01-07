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
	void AddInstance();

private:
	ModelSkinnedInstancing* modelInstancing;
	class EnemyInstance* instance;
	class IFocus* player;
};