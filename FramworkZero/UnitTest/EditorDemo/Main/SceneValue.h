#pragma once

struct SceneAction
{
	function<void(void)> Action;
	bool bDestroy = true;
};

struct SceneValueUnit
{
	map<string, void*>& InsertedValues;

	SceneAction* Updater;
	SceneAction* Renderer;
};

class SceneValue
{
public:
	SceneValue();
	~SceneValue();

public:
	void Initialize();
	void Update();
	void Render();

public:
	void AddObject(class IObjectSpawner* value);
	void AddValue(string tag, void* value);
	void* GetValue(string tag);

private:
	void Action(list<SceneAction*>& actions);

private:
	list<SceneAction*> updater;
	list<SceneAction*> renderer;
	map<string, void*> insertedValues;

	vector<class IObjectSpawner*> objects;
};
