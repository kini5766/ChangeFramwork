#pragma once

#include "EnemyDesc.h"

class MeleeEnemy : public IEnemy
{
public:
	MeleeEnemy(Shader* shader);
	~MeleeEnemy();

private:
	// IEnemy을(를) 통해 상속됨
	void Update() override;
	void Render() override;

	ModelSkinnedInstancing * GetModel() override;
	void BindAnimation(Animator *, ModelAnimation *) override;
	AttackAnimation * MakeAttackInstance(Transform *) override;
	const EnemyDesc * GetDesc() override;

private:
	ModelSkinnedInstancing* modelInstancing;
	class NormalAttack* normalAttack;
	EnemyDesc desc;
};
