#include "stdafx.h"
#include "IObjectSpawner.h"

#include "Utilities/BinaryFile.h"
#include "ObjectSpawnerFactory.h"


ObjectSpawner::ObjectSpawner(ObjectSpawnerFactory * factory)
	: factory(factory)
{
}

ObjectSpawner::~ObjectSpawner()
{
}

IObjectSpawner * ObjectSpawner::Load(BinaryReader * r)
{
}
