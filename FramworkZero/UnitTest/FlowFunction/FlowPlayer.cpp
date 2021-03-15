#include "stdafx.h"
#include "FlowFunction.h"

FlowPlayer::FlowPlayer()
	: funcNext(bind(&FlowPlayer::Next, this))
{
	FlowTesk::FuncCall = bind(&FlowPlayer::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&FlowPlayer::Update, this);
	FlowTesk::FuncCancel = bind(&FlowPlayer::Cancel, this);
}

FlowPlayer::~FlowPlayer()
{
}

void FlowPlayer::PushBack(FlowBase* function)
{
	funcStack.push_back(function);
}

void FlowPlayer::Clear()
{
	funcStack.clear();
}

void FlowPlayer::CancelNext()
{
	if (curr == nullptr)
		return;

	curr->FuncCancel();
	Next();
}

void FlowPlayer::HoldBackNext()
{
	if (curr == nullptr)
		return;

	FlowBase* hold = curr;
	Next();
	PushBack(hold);
}


void FlowPlayer::Call(const FutureAction * action)
{
	result.SetAction(action);
	Next();
}

void FlowPlayer::Update()
{
	if (curr == nullptr)
		return;

	curr->FuncUpdate();
}

void FlowPlayer::Cancel()
{
	result.Clear();
	funcStack.clear();

	if (curr == nullptr)
		return;

	curr->FuncCancel();
	curr = nullptr;
}


void FlowPlayer::Next()
{
	if (funcStack.size() == 0)
	{
		// 모든 함수 실행 완료 -> 종료
		curr = nullptr;
		result.OnAction();
		return;
	}

	FlowBase* next = funcStack.back();
	funcStack.pop_back();
	curr = nullptr;

	switch (next->GetType())
	{
	case FlowBase::ClassType::FlowAction:
		dynamic_cast<FlowAction*>(next)->FuncCall();
		Next(); 
		break;

	case FlowBase::ClassType::FlowTesk:
		curr = dynamic_cast<FlowTesk*>(next);
		curr->FuncCall(&funcNext);
		break;
	default: break;
	}
}
