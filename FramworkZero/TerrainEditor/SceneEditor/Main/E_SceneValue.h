#pragma once

struct E_SceneAction
{
	function<void(void)> Action;
	bool bDestroy = true;
};

struct E_SceneValueUnit
{
	map<string, void*>& InsertedValues;

	E_SceneAction* Updater;
	E_SceneAction* Renderer;
};

class E_SceneValue
{
public:
	E_SceneValue();
	~E_SceneValue();

public:
	void Update();
	void Render();

public:
	void AddValue(string tag, void* value);
	void* GetValue(string tag);
	E_SceneValueUnit AddUnit();

private:
	void Action(list<E_SceneAction*>& actions);

private:
	list<E_SceneAction*> updater;
	list<E_SceneAction*> renderer;
	map<string, void*> insertedValues;
};
