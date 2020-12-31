#include "stdafx.h"
#include "ObjectEditorFactory.h"

void ObjectEditorFactory::Insert()
{
	//creator.push_back(make_pair("Cube",
	//	[]()->IInspectorEditor* { return new CubeEditor(); }));
}

#include "Objects/MainCameraDebug.h"
void ObjectEditorFactory::InsertDebuger()
{
	debugerCreator.push_back(make_pair("MainCamera",
		[]()->IObjectDebuger* { return new MainCameraDebug(); }));
}


ObjectEditorFactory::ObjectEditorFactory()
{
	Insert();
	size = creator.size();
	names = new string[size];

	UINT size = creator.size();
	for (UINT i = 0; i < size; i++)
		names[i] = creator[i].first;
}

ObjectEditorFactory::~ObjectEditorFactory()
{
	SafeDeleteArray(names);
}

string * ObjectEditorFactory::GetNames(UINT * pSize)
{
	*pSize = size;
	return names;
}

IObjectEditor * ObjectEditorFactory::CreateEditor(string typeName)
{
	IObjectEditor* result = nullptr;

	for (auto c : creator)
	{
		if (typeName == c.first)
		{
			result = c.second();
			break;
		}
	}

	return result;
}

IObjectDebuger * ObjectEditorFactory::CreateDebuger(string typeName)
{
	IObjectDebuger* result = nullptr;

	for (auto c : debugerCreator)
	{
		if (typeName == c.first)
		{
			result = c.second();
			break;
		}
	}

	return result;
}
