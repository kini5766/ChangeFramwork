#include "stdafx.h"
#include "E_SceneValue.h"

E_SceneValue::E_SceneValue()
{
}

E_SceneValue::~E_SceneValue()
{
	for (auto d : updater)
		SafeDelete(d);

	for (auto d : renderer)
		SafeDelete(d);
}


void E_SceneValue::Update()
{
	Action(updater);
}

void E_SceneValue::Render()
{
	Action(renderer);
}

void E_SceneValue::AddValue(string tag, void * value)
{
	insertedValues[tag] = value;
}

void * E_SceneValue::GetValue(string tag)
{
	return insertedValues[tag];
}

E_SceneValueUnit E_SceneValue::AddUnit()
{
	E_SceneValueUnit result
	{
		insertedValues,
		new E_SceneAction(),
		new E_SceneAction()
	};

	updater.push_back(result.Updater);
	renderer.push_back(result.Renderer);
	return result;
}

void E_SceneValue::Action(list<E_SceneAction*>& actions)
{
	list<E_SceneAction*>::iterator iter = actions.begin();
	while (iter != actions.end())
	{
		if ((*iter)->bDestroy)
		{
			SafeDelete(*iter);
			actions.erase(iter++);
			continue;
		}

		(*iter)->Action();
		++iter;
	}
}
