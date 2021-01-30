#pragma once

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

public:
	void Load(class SceneValue* target, wstring file);

private:
	class IObjectSpawner* LoadObject(class BinaryReader * r, class ObjectSpawnerFactory* factory);
};
