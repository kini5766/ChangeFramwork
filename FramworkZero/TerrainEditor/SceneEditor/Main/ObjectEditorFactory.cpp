#include "stdafx.h"
#include "ObjectEditorFactory.h"

#include "Tools/Viewer/IFocus.h"

#include "IObjectEditor.h"
#include "../Objects/MeshInstancing/MeshInstancingEditor.h"

void ObjectEditorFactory::Insert()
{
	creators.push_back({ "MeshInstancing",
		[]()->IObjectEditor* { return new MeshInstancingEditor(); } });
}


ObjectEditorFactory::ObjectEditorFactory()
{
	Insert();
	size = creators.size();
	names = new string[size];

	for (UINT i = 0; i < size; i++)
		names[i] = creators[i].Name;
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

	for (auto c : creators)
	{
		if (typeName == c.Name)
		{
			result = c.Create();
			break;
		}
	}

	return result;
}
