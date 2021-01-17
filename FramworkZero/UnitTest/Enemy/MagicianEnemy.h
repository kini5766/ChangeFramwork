#pragma once

#include "EnemyDesc.h"

class MagicianEnemy : public IEnemy
{
public:
	MagicianEnemy(class IFocus* player);
	~MagicianEnemy();

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
	Shader* shaderMesh;

private:
	ModelSkinnedInstancing* modelInstancing;
	class MagicAttack* magicAttack;
	EnemyDesc desc;

	MeshInstancing* sphere;
};
