#include "Framework.h"
#include "TransformData.h"

TransformData::TransformData()
{
}

TransformData::~TransformData()
{
}

void TransformData::SetParent(TransformData * value)
{
	RemoveParent();
	if (value != nullptr)
		value->AddChild(this);
	parent = value;
}

void TransformData::RemoveParent()
{
	if (parent == nullptr)
		return;

	parent->RemoveChild(this);
}

bool TransformData::RemoveChild(TransformData * value)
{
	vector<TransformData*>::iterator iter = childs.begin();
	while (iter != childs.end())
	{
		if ((*iter) == value)
		{
			childs.erase(iter);
			return true;
		}

		++iter;
	}
	return false;
}

void TransformData::AddChild(TransformData * value)
{
	childs.push_back(value);
}
