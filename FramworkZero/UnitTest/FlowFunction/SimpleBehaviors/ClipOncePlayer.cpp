#include "stdafx.h"
#include "ClipOncePlayer.h"

ClipOncePlayer::ClipOncePlayer(const ClipPlayerDesc & desc)
	: desc(desc)
	, funcComple(bind(&ClipOncePlayer::CompleAnim, this))
{
	desc.Anim->AddFuncChange(desc.Clip, funcComple);
}

ClipOncePlayer::~ClipOncePlayer()
{
}


void ClipOncePlayer::Call(const FutureAction * action)
{
	if (result.IsValid())
	{
		// 재생 도중 다른 행동을 취하다 다시 온 경우

		result.Clear();
		(*action)();
		return;
	}

	result.Clear();
	result.SetAction(action);

	bChanged = false;
	desc.Anim->Play(desc.Clip);
}

void ClipOncePlayer::Update()
{
	if (bChanged)
	{
		bChanged = false;
		result.OnAction();
		return;
	}
}

void ClipOncePlayer::Cancel()
{
	result.Clear();
}


void ClipOncePlayer::CompleAnim()
{
	bChanged = true;
}
