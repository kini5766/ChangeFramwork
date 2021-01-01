#include "stdafx.h"
#include "ObjectEditorFactory.h"

#include "IObjectEditor.h"
#include "Objects/MeshInstancingEditor.h"

void ObjectEditorFactory::Insert()
{
	creator.push_back(make_pair("MeshInstancing",
		[]()->IObjectEditor* { return new MeshInstancingEditor(); }));
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
