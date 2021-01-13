#include "stdafx.h"
#include "ObjectEditorFactory.h"

#include "Tools/Viewer/IFocus.h"

#include "IObjectEditor.h"
#include "../Objects/MeshInstancingEditor.h"
#include "../Objects/EnemyInstancingEditor.h"

map<string, void*> ObjectEditorFactory::insertValues;

void ObjectEditorFactory::Insert()
{
	creator.push_back(make_pair("MeshInstancing",
		[]()->IObjectEditor* { return new MeshInstancingEditor(); }));
	creator.push_back(make_pair("EnemyInstancing",
		[]()->IObjectEditor* { return new EnemyInstancingEditor((IFocus*)insertValues["IFocusPlayer"]); }));
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

void ObjectEditorFactory::AddValue(string tag, void * value)
{
	insertValues[tag] = value;
}
