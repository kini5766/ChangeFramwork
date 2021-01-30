#pragma once

class ObjectSpawnerFactory
{
private:
	void Insert(class SceneValue* scene);

public:
	ObjectSpawnerFactory(class SceneValue* scene);
	~ObjectSpawnerFactory();

public:
	class IObjectSpawner* CreateSpawner(string typeName);

private:
	class SceneValue* scene;
	struct Creator
	{
		string Name;
		function<IObjectSpawner*()> Create;
	};
	vector<Creator> creators;
};
