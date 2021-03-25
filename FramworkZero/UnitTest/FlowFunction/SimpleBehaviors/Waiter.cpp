#include "stdafx.h"
#include "Waiter.h"


Waiter::Waiter(const WaiterDesc & desc)
	: desc(desc)
{
}

Waiter::~Waiter()
{
}

void Waiter::Call(const FutureAction * action)
{
	runningTime = 0.0f;

	result.SetAction(action);
}

void Waiter::Update()
{
	if (runningTime >= desc.Time)
	{
		result.OnAction();
		return;
	}

	runningTime += Time::Delta();
}

void Waiter::Cancel()
{
	result.Clear();
}