#include "stdafx.h"
#include "EnemyInstancing.h"

#include "Tools/Viewer/IFocus.h"
#include "EnemyInstance.h"

EnemyInstancing::EnemyInstancing(Shader * shader, IFocus* player)
	: player(player)
{
	modelInstancing = new ModelSkinnedInstancing(shader, {
			/*�Ž�*/ L"Paladin/Mesh",
			/*���͸���*/ L"Paladin/Mesh",
			/*Ŭ��*/ {
				L"Paladin/Idle",
				L"Paladin/Walk",
				L"Paladin/Run",
				L"Paladin/Taunt",
				L"Paladin/Attack",
				L"Paladin/React",
			}
		});


	AddInstance();

	modelInstancing->UpdateTransforms();
	modelInstancing->UpdateColors();
	modelInstancing->Pass(1);
}

EnemyInstancing::~EnemyInstancing()
{
	SafeDelete(instance);
	SafeDelete(modelInstancing);
}

void EnemyInstancing::Update()
{
	instance->Update();
	modelInstancing->Update();
	modelInstancing->UpdateTransforms();
}

void EnemyInstancing::Render()
{
	modelInstancing->Render();
	instance->Render();
}

void EnemyInstancing::AddInstance()
{
	instance = new EnemyInstance(modelInstancing->AddInstance(), player);
}
