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
	if (bCancel)
	{
		// ��� ���� �ٸ� �ൿ�� ���ϴ� �ٽ� �� ���
		bCancel = false;
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

void ClipOncePlayer::Hold()
{
	bCancel = true;
	result.Clear();
}


void ClipOncePlayer::CompleAnim()
{
	bChanged = true;
}
