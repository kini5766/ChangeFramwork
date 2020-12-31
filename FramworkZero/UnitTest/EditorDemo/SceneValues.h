#pragma once

#include "stdafx.h"

class IEditingObject
{
public:
	virtual ~IEditingObject() {};
	virtual void Render() = 0;
	virtual void Update() = 0;
};

struct SceneValues
{
	vector<IEditingObject*> Objs;
};

