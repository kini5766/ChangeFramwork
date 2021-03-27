#include "stdafx.h"
#include "V2EnemyMelee.h"

//#include "Tools/Coord/WorldMatrix.h"

#include "Character/Paladin.h"
#include "Skill/SkillMelee.h"
#include "V2EnemyDesc.h"
#include "FieldEnemy.h"

V2EnemyMelee::V2EnemyMelee(IFocus * player)
{
	model = new Paladin();
	enemyDesc = new FieldEnemyInput();
	skillDesc = new MeleeDesc();

	Transform world;
	world.Position(0.0f, 90.0f, -40.0f);
	world.Scale(100.0f, 180.0f, 180.0f);
	world.GlobalWorld(&skillDesc->InitMatrix);
	skillDesc->Tag = L"EnemyAttack";

	enemyDesc->Player = player;
	enemyDesc->Status.HP = 100.0f;
	enemyDesc->ClipGroup.ClipIdle = 0;
	enemyDesc->ClipGroup.ClipLookAround = 0;
	enemyDesc->ClipGroup.ClipWalk = 1;
	enemyDesc->ClipGroup.ClipRun = 2;
	enemyDesc->ClipGroup.ClipInSight = 3;
}

V2EnemyMelee::~V2EnemyMelee()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(skillDesc);
	SafeDelete(enemyDesc);
	SafeDelete(model);
}

void V2EnemyMelee::Update()
{
	for (FieldEnemy* i : instances)
		i->Update();

	//normalAttack->Update();
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
	SkillMelee* skill = new SkillMelee(*skillDesc, i->GetTransform());

	enemyDesc->Model = i;
	enemyDesc->localWorld = &localWorld;
	enemyDesc->PatrolPoints = patrolPoints;
	enemyDesc->Skill = skill;

	FieldEnemy* enemy = new FieldEnemy(*enemyDesc);

	instances.push_back(enemy);
}
