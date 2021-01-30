#include "stdafx.h"
#include "ObjectEditorList.h"

#include "ObjectEditor.h"

ObjectEditorList::ObjectEditorList()
{
}

ObjectEditorList::~ObjectEditorList()
{
	for (auto d : objs)
		SafeDelete(d);
}


UINT ObjectEditorList::Size() const
{
	return objs.size();
}

ObjectEditor * ObjectEditorList::GetObj(UINT index)
{
	return objs[index];
}

void ObjectEditorList::Clear()
{
	for (auto d : objs)
		SafeDelete(d);
	objs.clear();
}

ObjectEditor* ObjectEditorList::CreateEditor(EditorDesc* desc)
{
	ObjectEditor* obj = new ObjectEditor(desc);
	string str = obj->Name() + to_string(objs.size());
	obj->Name(str.c_str());
	objs.push_back(obj);
	return obj;
}

void ObjectEditorList::Destroy(int index)
{
	if (-1 == index)
		return;

	if (selectedNum == index)
		SelectNone();

	vector<ObjectEditor*>::iterator iter = objs.begin() + index;
	SafeDelete(*iter);
	objs.erase(iter);
}


void ObjectEditorList::SelectNone()
{
	if (IsSelected())
		objs[selectedNum]->Off();

	selectedNum = -1;
}

void ObjectEditorList::Select(int index)
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
