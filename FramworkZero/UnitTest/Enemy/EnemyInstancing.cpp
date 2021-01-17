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
	//SafeDelete(enemy);
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

	for (EnemyInstance* i : instances)
		i->Render();
}

void EnemyInstancing::AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints)
{
	ModelSkinnedInstance* i = enemy->GetModel()->AddInstance();

	Transform* t = i->GetTransform();
	t->LocalWorld(localWorld);

	Animator* anim = new Animator();
	enemy->BindAnimation(anim, i->GetAnimation());

	EnemyInstanceDesc desc;
	desc.Player = player;
	desc.Transform = t;
	desc.Animator = anim;
	desc.Attack = enemy->MakeAttackInstance(t);
	desc.PatrolPoints = patrolPoints;
	desc.Desc = enemy->GetDesc();
	instances.push_back(new EnemyInstance(&desc));
}
