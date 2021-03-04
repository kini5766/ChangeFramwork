#include "stdafx.h"
#include "MagicianEnemy.h"

#include "Rendering/Camera/Main/IFocus.h"
#include "Component/MagicAttack.h"

#include "Character/Friedrich.h"

MagicianEnemy::MagicianEnemy(IFocus* player)
{
	modelInstancing = new Friedrich();
	// instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	cubeMap = new Texture(L"Environment/GrassCube1024.dds");
	sphere = new MeshInstancing(unique_ptr<MeshData>(new MeshSphere(0.5f)));
	sphere->GetMaterial()->SetSRV("CubeMap", cubeMap->SRV());
	sphere->GetMaterial()->Emissive(0.9f, 0.9f, 0.9f, 0.9f);
	sphere->GetMaterial()->Specular(0.9f, 0.9f, 0.9f, 20.9f);
	sphere->GetMaterial()->Diffuse(0.0f, 0.0f, 0.0f);
	magicAttack = new MagicAttack(player, sphere);
	magicAttack->InitTransform()->Position(-25.0f, 135.0f, -40.0f);
	magicAttack->InitTransform()->RotationEuler(
		EulerAngle::Degree(180.0f, 0.0f, 0.0f));
	magicAttack->Tag(L"EnemyAttack");

	desc.HP = 80.0f;
	desc.AttackRange = 25.0f;
	desc.DetectionRange = 40.0f;
	//desc.TurnSpeed = 10.0f;

}

MagicianEnemy::~MagicianEnemy()
{
	SafeDelete(magicAttack);
	SafeDelete(sphere);
	SafeDelete(modelInstancing);

	SafeDelete(cubeMap);
}

void MagicianEnemy::Update()
{
	magicAttack->Update();
	modelInstancing->Update();
	modelInstancing->UpdateTransforms();

	sphere->Update();
	sphere->UpdateTransforms();
}

void MagicianEnemy::Render()
{
	modelInstancing->Render();
	sphere->Render_CubeMap();
}

ModelInstancing * MagicianEnemy::GetModel()
{
	return modelInstancing;
}

AttackAnimation * MagicianEnemy::MakeAttackInstance(Transform * t)
{
	return magicAttack->MakeAttackInstance(t);
}

const EnemyDesc * MagicianEnemy::GetDesc()
{
	return &desc;
}
