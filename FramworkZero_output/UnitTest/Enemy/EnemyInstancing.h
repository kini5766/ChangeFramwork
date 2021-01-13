#pragma once


class EnemyInstancing
{
public:
	EnemyInstancing(class IFocus* player, class IEnemy* desc);
	~EnemyInstancing();

public:
	void Update();
	void Render();

public:
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints);

private:
	class IEnemy* enemy;
	class IFocus* player;
	vector<class EnemyInstance*> instances;

	//class NormalAttack* normalAttack;
	//class MagicAttack* magicAttack;
	//MeshInstancing* sphere;
};