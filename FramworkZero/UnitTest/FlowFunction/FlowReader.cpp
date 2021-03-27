#include "stdafx.h"
#include "FlowFunction.h"

FlowReader::FlowReader()
	: funcNext(bind(&FlowReader::Next, this))
{
}

FlowReader::~FlowReader()
{
}

void FlowReader::PushBack(IFlowTesk* tesk)
{
	funcStack.push_back(tesk);
}

void FlowReader::PushBacks(UINT size, IFlowTesk** first)
{
	funcStack.insert(funcStack.end(), first, first + size);
}

void FlowReader::Reset()
{
	result.Clear();
	funcStack.clear();
	next = nullptr;

	if (curr == nullptr)
		return;

	curr->Hold();
	curr = nullptr;
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

	curr->Hold();
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

	IFlowTesk* hold = curr;
	hold->Hold();
	Next();
	PushBack(hold);
}


void FlowReader::Call(const FutureAction * action)
{
	result.SetAction(action);

	if (curr != nullptr)
	{
		next = curr;
		curr = nullptr;
	}
	else Next();
}

void FlowReader::Update()
{
	if (next != nullptr)
	{
		curr = next;
		next = nullptr;
		curr->Call(&funcNext);
	}

	if (curr == nullptr)
		return;

	curr->Update();
}

void FlowReader::Hold()
{
	result.Clear();

	if (curr == nullptr)
		return;

	curr->Hold();
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
