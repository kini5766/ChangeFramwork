#pragma once

class V2EnemyMelee
{
public:
	V2EnemyMelee(class IFocus* player);
	~V2EnemyMelee();


public:
	void Update();
	void Render();
	void PostRender();

public:
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints);

private:
	ModelInstancing* model;
	struct FieldEnemyInput* enemyDesc;
	struct MeleeDesc* skillDesc;

private:
	vector<class FieldEnemy*> instances;
	vector<class SkillMelee*> skills;
};
