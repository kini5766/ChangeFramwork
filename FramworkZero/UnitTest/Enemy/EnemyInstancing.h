#pragma once


class EnemyInstancing
{
public:
	EnemyInstancing(class IFocus* player, unique_ptr<class IEnemy> desc);
	~EnemyInstancing();

public:
	void Update();
	void Render();
	void PostRender();

public:
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints);

private:
	class IFocus* player;
	unique_ptr<class IEnemy> enemy;
	vector<class EnemyInstance*> instances;
};