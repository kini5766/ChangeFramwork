#include "stdafx.h"
#include "SceneLoader.h"

#include "Utilities/BinaryFile.h"

#include "SceneValue.h"
#include "IObjectSpawner.h"
#include "ObjectSpawnerFactory.h"


SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
}

void SceneLoader::Load(SceneValue * target, wstring file)
{
	ObjectSpawnerFactory factory = ObjectSpawnerFactory(target);
	IObjectSpawner* spawner;

	BinaryReader r(URI::Scenes + file + L".scene");
	UINT size = r.UInt();
	for (UINT i = 0; i < size; i++)
	{
		spawner = LoadObject(&r, &factory);
		if (spawner != nullptr)
			target->AddObject(spawner);
	}
}

IObjectSpawner * SceneLoader::LoadObject(BinaryReader * r, ObjectSpawnerFactory * factory)
{
	string typeName = r->String();
	string name = r->String();

	if (typeName == "None")
		return nullptr;

	IObjectSpawner * target;
	target = factory->CreateSpawner(typeName);

	if (target == nullptr)
		return nullptr;

	if (target->Load(r))
		return target;

	SafeDelete(target);
	return nullptr;
}
