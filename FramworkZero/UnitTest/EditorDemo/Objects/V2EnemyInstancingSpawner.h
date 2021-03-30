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
	void PostRender();

private:
	// IObjectSpawner��(��) ���� ��ӵ�
	bool Load(BinaryReader * r) override;
	void Initialize(SceneValueUnit * scene) override;

private:
	void CreateEnemy(int item);
	void SetScale(Transform * t);

private:
	class EnemyMakerBase* enemy = nullptr;
	class IFocus* player;

private:
	// 0 : Melee, 1 : Magician
	int enemyType = 0;
};