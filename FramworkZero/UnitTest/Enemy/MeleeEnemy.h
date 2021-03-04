#pragma once

#include "EnemyDesc.h"

class MeleeEnemy : public IEnemy
{
public:
	MeleeEnemy();
	~MeleeEnemy();

private:
	// IEnemy을(를) 통해 상속됨
	void Update() override;
	void Render() override;

	ModelInstancing * GetModel() override;
	AttackAnimation * MakeAttackInstance(Transform *) override;
	const EnemyDesc * GetDesc() override;

private:
	ModelInstancing* modelInstancing;
	class NormalAttack* normalAttack;
	EnemyDesc desc;
};
