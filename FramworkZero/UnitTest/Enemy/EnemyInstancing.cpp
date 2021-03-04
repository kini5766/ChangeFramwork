#include "stdafx.h"
#include "EnemyInstancing.h"

#include "Enemy/EnemyDesc.h"

#include "Component/MagicAttack.h"
#include "EnemyInstance.h"


EnemyInstancing::EnemyInstancing(IFocus* player, unique_ptr<IEnemy> enemy)
	: player(player), enemy(move(enemy))
{
}

EnemyInstancing::~EnemyInstancing()
{
	for (auto d : instances)
		SafeDelete(d);
	enemy.reset();
}

void EnemyInstancing::Update()
{
	for (EnemyInstance* i : instances)
		i->Update();

	enemy->Update();
}

void EnemyInstancing::Render()
{
	enemy->Render();
}

void EnemyInstancing::PostRender()
{
	for (EnemyInstance* i : instances)
		i->PostRender();
}

void EnemyInstancing::AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints)
{
	ModelInstance* i = enemy->GetModel()->AddInstance();

	Transform* t = i->GetTransform();
	t->LocalWorld(localWorld);

	EnemyInstanceDesc desc;
	desc.Player = player;
	desc.Transform = t;
	desc.Animator = i->GetAnimator();
	desc.Attack = enemy->MakeAttackInstance(t);
	desc.PatrolPoints = patrolPoints;
	desc.Desc = enemy->GetDesc();
	instances.push_back(new EnemyInstance(&desc));
}
