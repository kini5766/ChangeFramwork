#include "stdafx.h"
#include "SceneValue.h"

#include "IObjectSpawner.h"

SceneValue::SceneValue()
{
}

SceneValue::~SceneValue()
{
	for (auto d : updater)
		SafeDelete(d);

	for (auto d : renderer)
		SafeDelete(d);

	for (auto d : objects)
		SafeDelete(d);
}

void SceneValue::Initialize()
{
	for (IObjectSpawner* object : objects)
	{
		SceneValueUnit unit = {
			insertedValues,
			new SceneAction(),
			new SceneAction()
		};

		object->Initialize(&unit);

		if (unit.Updater->bDestroy)
		{
			SafeDelete(unit.Updater);
		}
		else updater.push_back(unit.Updater);

		if (unit.Renderer->bDestroy)
		{
			SafeDelete(unit.Renderer);
		}
		else renderer.push_back(unit.Renderer);
	}

}

void SceneValue::Update()
{
	Action(updater);
}

void SceneValue::Render()
{
	Action(renderer);
}

void SceneValue::AddObject(IObjectSpawner * value)
{
	objects.push_back(value);
}

void SceneValue::AddValue(string tag, void * value)
{
	insertedValues[tag] = value;
}

void * SceneValue::GetValue(string tag)
{
	return insertedValues[tag];
}

void SceneValue::Action(list<SceneAction*>& actions)
{
	list<SceneAction*>::iterator iter = actions.begin();
	while (iter != actions.end())
	{
		if ((*iter)->bDestroy)
		{
			SafeDelete(*iter);
			actions.erase(iter++);
			return;
		}

		(*iter)->Action();
		++iter;
	}
}
