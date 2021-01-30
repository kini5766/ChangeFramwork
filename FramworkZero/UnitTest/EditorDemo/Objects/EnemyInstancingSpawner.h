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

private:
	// IObjectSpawner을(를) 통해 상속됨
	virtual bool Load(BinaryReader * r) override;
	virtual void Initialize(SceneValueUnit * scene) override;

private:
	void CreateEnemy(int item);
	void SetScale(Transform * t);

private:
	// render
	class EnemyInstancing* enemy;
	class IFocus* player;

private:
	// load
	// 0 : Melee, 1 : Magician
	int enemyType = 0;
	Transform* modelTemp = nullptr;
	
	struct EnemyInstanceDesc
	{
		Transform Transform;
		vector<Vector3> PatrolPoints;
	};
	vector<EnemyInstanceDesc*> descs;
};