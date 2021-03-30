#include "stdafx.h"
#include "V2EnemyMagic.h"

//#include "Tools/Coord/WorldMatrix.h"

#include "Character/Friedrich.h"
#include "Skill/MagicMaker.h"
#include "Skill/SkillMagic.h"
#include "V2EnemyDesc.h"
#include "FieldEnemy.h"

V2EnemyMagic::V2EnemyMagic(IFocus * player)
{
	model = new Friedrich();
	enemyDesc = new FieldEnemyInput();

	cubeMap = new Texture(L"Environment/GrassCube1024.dds");
	magicBullet = new MeshInstancing(unique_ptr<MeshData>(new MeshSphere(0.5f)));
	skillDesc = new MagicDesc();
	skillMaker = new MagicMaker(magicBullet);

	// -- magicBullet -- //
	magicBullet->GetMaterial()->SetSRV("CubeMap", cubeMap->SRV());
	magicBullet->GetMaterial()->Emissive(0.9f, 0.9f, 0.9f, 0.9f);
	magicBullet->GetMaterial()->Specular(0.9f, 0.9f, 0.9f, 20.9f);
	magicBullet->GetMaterial()->Diffuse(0.0f, 0.0f, 0.0f);

	// -- enemyDesc -- //
	Transform world;
	world.Position(-25.0f, 135.0f, -40.0f);
	world.RotationEuler(
		EulerAngle::Degree(180.0f, 0.0f, 0.0f));
	world.GlobalWorld(&skillDesc->InitMatrix);
	enemyDesc->Player = player;
	enemyDesc->Status.HP = 80.0f;
	enemyDesc->Status.MinRange = 19.0f;
	enemyDesc->ClipGroup.ClipIdle = 0;
	enemyDesc->ClipGroup.ClipLookAround = 0;
	enemyDesc->ClipGroup.ClipWalk = 1;
	enemyDesc->ClipGroup.ClipRun = 2;
	enemyDesc->ClipGroup.ClipInSight = 3;

	// -- skillDesc -- //
	skillDesc->Maker = skillMaker;
	skillDesc->Player = player;
	skillDesc->Tag = L"EnemyAttack";
	skillDesc->ClipAttack = 4;
	skillDesc->ClipRun = enemyDesc->ClipGroup.ClipRun;
	skillDesc->ClipIdle = enemyDesc->ClipGroup.ClipIdle;
	skillDesc->RunSpeed = enemyDesc->Status.RunSpeed;
}

V2EnemyMagic::~V2EnemyMagic()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(magicBullet);
	SafeDelete(skillMaker);
	SafeDelete(skillDesc);
	SafeDelete(enemyDesc);
	SafeDelete(cubeMap);
	SafeDelete(model);
}

void V2EnemyMagic::Update()
{
	for (FieldEnemy* i : instances)
		i->Update();

	skillMaker->Update();

	model->Update();
	model->UpdateTransforms();

	magicBullet->Update();
	magicBullet->UpdateTransforms();
}

void V2EnemyMagic::Render()
{
	model->Render();
	magicBullet->Render_CubeMap();
}

void V2EnemyMagic::PostRender()
{
	for (FieldEnemy* i : instances)
		i->PostRender();
}

void V2EnemyMagic::AddInstance(const Matrix & localWorld, const vector<Vector3>* patrolPoints)
{
	ModelInstance* i = model->AddInstance();
	SkillMagic* skill = new SkillMagic(*skillDesc, i);

	enemyDesc->Model = i;
	enemyDesc->localWorld = &localWorld;
	enemyDesc->PatrolPoints = patrolPoints;
	enemyDesc->Skill = skill;

	FieldEnemy* enemy = new FieldEnemy(*enemyDesc);

	instances.push_back(enemy);
}
