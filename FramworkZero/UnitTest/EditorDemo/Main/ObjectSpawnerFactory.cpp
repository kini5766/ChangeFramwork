#include "stdafx.h"
#include "ObjectSpawnerFactory.h"

#include "SceneValue.h"
#include "IObjectSpawner.h"

#include "../Objects/MeshInstancingSpawner.h"
#include "../Objects/EnemyInstancingSpawner.h"


void ObjectSpawnerFactory::Insert(SceneValue* scene)
{
	creators.push_back({ "MeshInstancing",
		[]()->IObjectSpawner* { return new MeshInstancingSpawner(); } });
	creators.push_back({ "EnemyInstancing",
		[scene]()->IObjectSpawner* { return new EnemyInstancingSpawner(scene); } });
}


ObjectSpawnerFactory::ObjectSpawnerFactory(SceneValue* scene)
	: scene(scene)
{
	Insert(scene);
}

ObjectSpawnerFactory::~ObjectSpawnerFactory()
{
}

IObjectSpawner * ObjectSpawnerFactory::CreateSpawner(string typeName)
{
	IObjectSpawner* result = nullptr;

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

