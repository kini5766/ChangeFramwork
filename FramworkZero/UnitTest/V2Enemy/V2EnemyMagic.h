#pragma once

class V2EnemyMagic
{
public:
	V2EnemyMagic(class IFocus* player);
	~V2EnemyMagic();


public:
	void Update();
	void Render();
	void PostRender();

public:
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints);

private:
	Texture* cubeMap;
	ModelInstancing* model;
	struct FieldEnemyInput* enemyDesc;
	struct MagicDesc* skillDesc;
	
	class MeshInstancing* magicBullet;
	class MagicMaker* skillMaker;

private:
	vector<class FieldEnemy*> instances;
	vector<class SkillMagic*> skills;
};
