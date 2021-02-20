#pragma once

#include "EnemyDesc.h"

class MeleeEnemy : public IEnemy
{
public:
	MeleeEnemy();
	~MeleeEnemy();

private:
	// IEnemy��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;

	ModelInstancing * GetModel() override;
	void BindAnimation(Animator *, AnimationAdapter *) override;
	AttackAnimation * MakeAttackInstance(Transform *) override;
	const EnemyDesc * GetDesc() override;

private:
	ModelInstancing* modelInstancing;
	class NormalAttack* normalAttack;
	EnemyDesc desc;
};
