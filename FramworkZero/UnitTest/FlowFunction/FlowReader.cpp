#include "stdafx.h"
#include "FlowFunction.h"

FlowReader::FlowReader()
	: funcNext(bind(&FlowReader::Next, this))
{
	FlowTesk::FuncCall = bind(&FlowReader::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&FlowReader::Update, this);
	FlowTesk::FuncCancel = bind(&FlowReader::Cancel, this);
}

FlowReader::~FlowReader()
{
}

void FlowReader::PushBack(FlowTesk* tesk)
{
	funcStack.push_back(tesk);
}

void FlowReader::PushBacks(UINT size, FlowTesk* first)
{
	funcStack.insert(funcStack.end(), first, first + size);
}

void FlowReader::Clear()
{
	funcStack.clear();
}

void FlowReader::CancelNext()
{
	if (next != nullptr)
	{
		next = nullptr;
		Next();
		return;
	}

	if (curr == nullptr)
		return;

	curr->FuncCancel();
	Next();
}

void FlowReader::HoldBackNext()
{
	if (next != nullptr)
	{
		// next값이 존재하면 curr값은 무조건 nullptr
		curr = next;
		next = nullptr;
	}

	if (curr == nullptr)
		return;

	FlowTesk* hold = curr;
	Next();
	PushBack(hold);
}


void FlowReader::Call(const FutureAction * action)
{
	result.SetAction(action);
	Next();
}

void FlowReader::Update()
{
	if (next != nullptr)
	{
		curr = next;
		next = nullptr;
		curr->FuncCall(&funcNext);
	}

	if (curr == nullptr)
		return;

	curr->FuncUpdate();
}

void FlowReader::Cancel()
{
	result.Clear();
	funcStack.clear();

	if (curr == nullptr)
		return;

	curr->FuncCancel();
	curr = nullptr;
	next = nullptr;
}


void FlowReader::Next()
{
	curr = nullptr;

	if (funcStack.size() == 0)
	{
		// 모든 함수 실행 완료 -> 종료
		result.OnAction();
		return;
	}

	next = funcStack.back();
	funcStack.pop_back();
}
