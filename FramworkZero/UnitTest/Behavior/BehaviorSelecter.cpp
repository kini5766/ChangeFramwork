#include "stdafx.h"
#include "BehaviorSelecter.h"

BehaviorSelecter::BehaviorSelecter()
{
}

BehaviorSelecter::~BehaviorSelecter()
{
}

UINT BehaviorSelecter::GetNextBehavior()
{
	UINT i = 0;
	for (; i < behaviors.size(); i++)
		if (behaviors[i]())
			return i;

	return i;
}
