#include "stdafx.h"
#include "EditorValue.h"

#include "IObjectEditor.h"

EditorValue::EditorValue()
{
}

EditorValue::~EditorValue()
{
	for (auto d : objs)
		SafeDelete(d);
}

void EditorValue::Update()
{
	for (ObjectEditor* obj : objs)
		obj->Update();
}

void EditorValue::Render()
{
	for (ObjectEditor* obj : objs)
		obj->Render();
}

void EditorValue::ImGuiRender()
{
	if (selectedNum != -1)
		objs[selectedNum]->ImGuiRender();
}


UINT EditorValue::Size() const
{
	return objs.size();
}

ObjectEditor * EditorValue::GetObj(UINT index)
{
	return objs[index];
}

void EditorValue::Clear()
{
	for (auto d : objs)
		SafeDelete(d);
	objs.clear();
}


void EditorValue::Add(ObjectEditor * obj)
{
	objs.push_back(obj);
}

void EditorValue::Destroy(int index)
{
	if (-1 == index)
		return;

	if (selectedNum == index)
		SelectNone();

	vector<ObjectEditor*>::iterator iter = objs.begin() + index;
	SafeDelete(*iter);
	objs.erase(iter);
}


void EditorValue::SelectNone()
{
	if (IsSelected())
		objs[selectedNum]->Off();

	selectedNum = -1;
}

void EditorValue::Select(int index)
{
	if (selectedNum == index ||
		-1 == index)
	{
		SelectNone();
		return;
	}

	if (IsSelected())
		objs[selectedNum]->Off();

	selectedNum = index;
	objs[selectedNum]->On();
}
