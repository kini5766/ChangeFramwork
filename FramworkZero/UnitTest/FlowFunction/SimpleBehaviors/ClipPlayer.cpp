#include "stdafx.h"
#include "ClipPlayer.h"

ClipPlayer::ClipPlayer(const ClipPlayerDesc & desc)
	: desc(desc)
{
	FlowTesk::FuncCall = bind(&ClipPlayer::Call, this, placeholders::_1);
}

ClipPlayer::~ClipPlayer()
{
}

void ClipPlayer::Call(const FutureAction* future)
{
	desc.Anim->Play(desc.Clip);
	(*future)();
}
