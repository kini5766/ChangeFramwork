#include "stdafx.h"
#include "Friedrich.h"

Friedrich::Friedrich()
	: ModelInstancing({
	/*매쉬*/ L"Friedrich/Mesh",
	/*매터리얼*/ L"Friedrich/Mesh",
	/*클립*/ {
		L"Friedrich/Idle",  // 0
		L"Friedrich/Walk",  // 1
		L"Friedrich/Run",  // 2
		L"Friedrich/Taunt",  // 3
		L"Friedrich/Attack",  // 4
		L"Friedrich/React",  // 5
		L"Friedrich/Fall",  // 6
		}
	})
{
	AnimData* anim = GetAnimData();
	// 0 Idle
	// 1 Walk
	// 2 Run

	// 3 Taunt
	anim->Clips[3].Blends[2].bDefault = true;
	anim->Clips[3].Blends[3].bDefault = false;

	// 4 Attack
	anim->Clips[4].Speed = 2.0f;
	anim->Clips[4].Blends[0].bDefault = true;
	anim->Clips[4].Blends[4].bDefault = false;

	// 5 React
	anim->Clips[5].Blends[0].bDefault = true;
	anim->Clips[5].Blends[5].bDefault = false;

	// instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

Friedrich::~Friedrich()
{
}
