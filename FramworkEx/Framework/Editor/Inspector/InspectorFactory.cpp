#include "Framework.h"
#include "InspectorFactory.h"
#include "IInspectorEditor.h"

Shader* InspectorFactory::shaderModel = nullptr;
Shader* InspectorFactory::shaderMesh = nullptr;
Shader* InspectorFactory::shaderTerrain = nullptr;

void InspectorFactory::CreateArgs()
{
	if (shaderModel != nullptr)
		return;

	shaderModel = new Shader(L"18_Animation.fx");
	shaderMesh = new Shader(L"10_Mesh.fx");
	shaderTerrain = new Shader(L"12_TerrainBrush.fx");
}

void InspectorFactory::DeleteArgs()
{
	SafeDelete(shaderTerrain);
	SafeDelete(shaderMesh);
	SafeDelete(shaderModel);
}


InspectorFactory::InspectorFactory()
{
	CreateArgs();
	Insert();
	size = creator.size();
	names = new string[size];

	UINT i = 0;
	for (auto c : creator)
	{
		names[i] = c.first;
		i++;
	}
}

InspectorFactory::~InspectorFactory()
{
	DeleteArgs();
	SafeDeleteArray(names);
}

string* InspectorFactory::GetNames(UINT * pSize)
{
	*pSize = size;
	return names;
}

IInspectorEditor * InspectorFactory::CreateEditor(string typeName)
{
	IInspectorEditor* result = nullptr;

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

