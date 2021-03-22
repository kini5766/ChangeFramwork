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
	result.Clear();
	result.SetAction(action);

	bChanged = false;
	desc.Anim->Play(desc.Clip);
}

void ClipOncePlayer::Cancel()
{
	result.Clear();
}

void ClipOncePlayer::CompleAnim()
{
	result.OnAction();
}
