#include "stdafx.h"
#include "EnemyInstancing.h"

#include "Tools/Viewer/IFocus.h"
#include "EnemyInstance.h"

EnemyInstancing::EnemyInstancing(Shader * shader, IFocus* player)
	: player(player)
{
	modelInstancing = new ModelSkinnedInstancing(shader, {
			/*매쉬*/ L"Paladin/Mesh",
			/*매터리얼*/ L"Paladin/Mesh",
			/*클립*/ {
				L"Paladin/Idle",  // 0
				L"Paladin/Walk",  // 1
				L"Paladin/Run",  // 2
				L"Paladin/Taunt",  // 3
				L"Paladin/Attack",  // 4
				L"Paladin/React",  // 5
				L"Paladin/Fall",  // 6
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
