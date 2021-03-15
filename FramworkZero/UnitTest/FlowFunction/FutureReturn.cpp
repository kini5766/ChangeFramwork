#include "stdafx.h"
#include "FlowFunction.h"

FutureReturn::FutureReturn()
{
}

FutureReturn::~FutureReturn()
{
}

void FutureReturn::SetAction(const FutureAction * value)
{
	action = value;
}

void FutureReturn::OnAction()
{
	const FutureAction * temp = action;
	action = nullptr;
	if (temp) (*temp)();
}
