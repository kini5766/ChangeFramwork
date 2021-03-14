#include "stdafx.h"
#include "DelayFunction.h"
//#include "DelayReturn.h"

DelayReturn::DelayReturn()
{
}

DelayReturn::~DelayReturn()
{
}

void DelayReturn::SetAction(const ReturnAction * value)
{
	action = value;
}

void DelayReturn::OnAction()
{
	const ReturnAction * temp = action;
	action = nullptr;
	if (temp) (*temp)();
}
