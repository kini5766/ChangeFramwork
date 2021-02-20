#pragma once

#include "EnemyDesc.h"

class MagicianEnemy : public IEnemy
{
public:
	MagicianEnemy(class IFocus* player);
	~MagicianEnemy();

private:
	// IEnemy을(를) 통해 상속됨
	void Update() override;
	void Render() override;

	ModelInstancing * GetModel() override;
	void BindAnimation(Animator *, AnimationAdapter *) override;
	AttackAnimation * MakeAttackInstance(Transform *) override;
	const EnemyDesc * GetDesc() override;

private:
	ModelInstancing* modelInstancing;
	class MagicAttack* magicAttack;
	EnemyDesc desc;

	MeshInstancing* sphere;
	Texture* cubeMap;
};
