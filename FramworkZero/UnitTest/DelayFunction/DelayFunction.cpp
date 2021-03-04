#include "stdafx.h"
#include "DelayFunction.h"

DelayReader::DelayReader()
{
	mineResult = new DelayReturn(
		bind(&DelayReader::Next, this)
	);
}

DelayReader::~DelayReader()
{
	SafeDelete(mineResult);
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


void DelayReader::Call(const DelayReturn * _result)
{
	result = _result;
}

void DelayReader::Update()
{
	if (result == nullptr)
		return;

	curr->Update();
}

void DelayReader::Cancel()
{
	result = nullptr;
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
		// 모든 함수 실행 완료 -> 종료
		(*result)();
		result = nullptr;
		curr = nullptr;
		return;
	}

	curr = funcStack.back();
	funcStack.pop_back();

	curr->Call(mineResult);
}
