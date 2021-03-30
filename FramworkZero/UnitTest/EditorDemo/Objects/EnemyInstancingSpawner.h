#pragma once

#include "../Main/IObjectSpawner.h"

class EnemyInstancingSpawner : public IObjectSpawner
{
public:
	EnemyInstancingSpawner(SceneValue* scene);
	~EnemyInstancingSpawner();

private:
	void Update();
	void Render();
	void PostRender();

private:
	// IObjectSpawner을(를) 통해 상속됨
	bool Load(BinaryReader * r) override;
	void Initialize(SceneValueUnit * scene) override;

private:
	void CreateEnemy(int item);
	void SetScale(Transform * t);

private:
	class EnemyInstancing* enemy;
	class IFocus* player;

private:
	// 0 : Melee, 1 : Magician
	int enemyType = 0;
};