#include "stdafx.h"
#include "SceneValue.h"

#include "IObjectEditor.h"

SceneValue::SceneValue()
{
}

SceneValue::~SceneValue()
{
	for (auto d : objs)
		SafeDelete(d);
}

void SceneValue::Update()
{
	for (ObjectEditor* obj : objs)
		obj->Update();
}

void SceneValue::Render()
{
	for (ObjectEditor* obj : objs)
		obj->Render();
}

void SceneValue::Add(ObjectEditor * obj)
{
	objs.push_back(obj);
}

void SceneValue::Destroy(UINT index)
{
	vector<ObjectEditor*>::iterator iter = objs.begin() + index;
	SafeDelete(*iter);
	objs.erase(iter);
}

UINT SceneValue::Size() const
{
	return objs.size();
}

ObjectEditor * SceneValue::Obj(UINT index)
{
	return objs[index];
}

void SceneValue::Clear()
{
	for (auto d : objs)
		SafeDelete(d);
	objs.clear();
}
