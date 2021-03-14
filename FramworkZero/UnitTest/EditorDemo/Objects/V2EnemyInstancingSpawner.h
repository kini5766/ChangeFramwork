#pragma once

#include "../Main/IObjectSpawner.h"

class V2EnemyInstancingSpawner : public IObjectSpawner
{
public:
	V2EnemyInstancingSpawner(SceneValue* scene);
	~V2EnemyInstancingSpawner();

private:
	void Update();
	void Render();

private:
	// IObjectSpawner을(를) 통해 상속됨
	bool Load(BinaryReader * r) override;
	void Initialize(SceneValueUnit * scene) override;

private:
	void CreateEnemy(int item);
	void SetScale(Transform * t);

private:
	class V2EnemyMelee* enemy;
	class IFocus* player;

private:
	// 0 : Melee, 1 : Magician
	int enemyType = 0;
};