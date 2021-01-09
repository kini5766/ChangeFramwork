#pragma once

#include "EnemyDesc.h"

class MagicianEnemy : public IEnemy
{
public:
	MagicianEnemy(Shader* shader, class IFocus* player);
	~MagicianEnemy();

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
	class MagicAttack* magicAttack;
	EnemyDesc desc;

	MeshInstancing* sphere;
};
