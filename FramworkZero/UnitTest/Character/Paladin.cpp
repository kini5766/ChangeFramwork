#include "stdafx.h"
#include "Paladin.h"


// --
// Paladin
// --

Paladin::Paladin()
	: ModelInstancing({
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
	})
{
	AnimData* anim = GetAnimData();

	// 0 Idle
	anim->Clips[0].Blends[2].TweeningTime = 0.0f;
	// 1 Walk
	// 2 Run
	anim->Clips[2].Blends[0].TweeningTime = 0.0f;
	anim->Clips[2].Blends[4].TweeningTime = 0.0f;
	//anim->Clips[2].Blends[5].TweeningTime = 0.0f;

	// 3 Taunt
	anim->Clips[3].Blends[2].bDefault = true;
	anim->Clips[3].Blends[3].bDefault = false;
	anim->Clips[3].Blends[2].TweeningTime = 0.0f;

	// 4 Attack
	anim->Clips[4].Speed = 2.0f;
	anim->Clips[4].Blends[0].bDefault = true;
	anim->Clips[4].Blends[4].bDefault = false;
	anim->Clips[4].Blends[2].TweeningTime = 0.0f;

	// 5 React
	anim->Clips[5].Blends[0].bDefault = true;
	anim->Clips[5].Blends[5].bDefault = false;
	//instance->GetTransform()->Scale(0.03f, 0.03f, 0.03f);
}

Paladin::~Paladin()
{
}
