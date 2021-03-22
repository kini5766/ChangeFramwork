#include "stdafx.h"
#include "ClipPlayer.h"

ClipPlayer::ClipPlayer(const ClipPlayerDesc & desc)
	: desc(desc)
{
}

ClipPlayer::~ClipPlayer()
{
}

void ClipPlayer::Call(const FutureAction* future)
{
	desc.Anim->Play(desc.Clip);
	(*future)();
}
