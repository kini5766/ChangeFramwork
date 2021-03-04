#include "stdafx.h"
#include "Kachujin.h"

Kachujin::Kachujin()
	: ModelInstancing({
	/*�Ž�*/ L"Kachujin/Mesh",
	/*���͸���*/ L"Kachujin/Mesh",
	/*Ŭ��*/ {
		L"Kachujin/Idle",  // 1
		L"Kachujin/Walk",  // 2
		L"Kachujin/Run",  // 3
		L"Kachujin/Slash",  // 4
		L"Kachujin/Fall",  // 5
		}
	})
{
	AnimData* anim = GetAnimData();

	anim->Clips[3].Speed = 2.0f;
	anim->Clips[3].Blends[0].bDefault = true;
	anim->Clips[3].Blends[3].bDefault = false;

	//instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

Kachujin::~Kachujin()
{
}

