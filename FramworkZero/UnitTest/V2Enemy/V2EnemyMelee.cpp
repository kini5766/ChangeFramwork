#include "stdafx.h"
#include "V2EnemyMelee.h"

#include "Character/Paladin.h"
#include "V2EnemyDesc.h"
#include "FieldEnemy.h"

V2EnemyMelee::V2EnemyMelee(IFocus * player)
{
	model = new Paladin();

	desc = new FieldEnemyDesc();
	desc->Player = player;
	desc->Status.HP = 100.0f;
	desc->ClipGroup.ClipIdle = 0;
	desc->ClipGroup.ClipLookAround = 0;
	desc->ClipGroup.ClipWalk = 1;
	desc->ClipGroup.ClipRun = 2;
	desc->ClipGroup.ClipInSight = 3;
}

V2EnemyMelee::~V2EnemyMelee()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(desc);
	SafeDelete(model);
}

void V2EnemyMelee::Update()
{
	for (FieldEnemy* i : instances)
		i->Update();

	model->Update();
	model->UpdateTransforms();
}

void V2EnemyMelee::Render()
{
	model->Render();
}

void V2EnemyMelee::PostRender()
{
	for (FieldEnemy* i : instances)
		i->PostRender();
}

void V2EnemyMelee::AddInstance(const Matrix & localWorld, const vector<Vector3>* patrolPoints)
{
	ModelInstance* i = model->AddInstance();

	FieldEnemyInput input;
	input.Desc = desc;
	input.Model = i;
	input.localWorld = &localWorld;
	input.PatrolPoints = patrolPoints;

	FieldEnemy* enemy = new FieldEnemy(input);

	instances.push_back(enemy);
}
