#include "stdafx.h"
#include "DelayFunction.h"
//#include "DelayReader.h"

DelayReader::DelayReader()
	: funcNext(bind(&DelayReader::Next, this))
{
}

DelayReader::~DelayReader()
{
}

void DelayReader::PushBack(IDelayFunction* function)
{
	funcStack.push_back(function);
}

void DelayReader::Clear()
{
	funcStack.clear();
}

void DelayReader::CancelNext()
{
	if (curr == nullptr)
		return;

	curr->Cancel();
	Next();
}

void DelayReader::HoldBackNext()
{
	if (curr == nullptr)
		return;

	IDelayFunction* hold = curr;
	Next();
	PushBack(hold);
}


void DelayReader::Call(const ReturnAction * action)
{
	result.SetAction(action);
	Next();
}

void DelayReader::Update()
{
	if (curr == nullptr)
		return;

	curr->Update();
}

void DelayReader::Cancel()
{
	result.Clear();
	funcStack.clear();

	if (curr == nullptr)
		return;

	curr->Cancel();
	curr = nullptr;
}


void DelayReader::Next()
{
	if (funcStack.size() == 0)
	{
		curr = nullptr;

		// 모든 함수 실행 완료 -> 종료
		result.OnAction();
		return;
	}

	curr = funcStack.back();
	funcStack.pop_back();

	curr->Call(&funcNext);
}
