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

	ModelSkinnedInstancing * GetModel() override;
	void BindAnimation(Animator *, ModelAnimation *) override;
	AttackAnimation * MakeAttackInstance(Transform *) override;
	const EnemyDesc * GetDesc() override;

private:
	Shader* shaderModel;

private:
	ModelSkinnedInstancing* modelInstancing;
	class NormalAttack* normalAttack;
	EnemyDesc desc;
};
