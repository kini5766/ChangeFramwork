#pragma once

#include "Framework.h"

class IFocus
{
public:
	virtual void Focus(Vector3* outPosition) = 0;
};