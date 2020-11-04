#pragma once

class SceneValueEditor
{
public:
	SceneValueEditor();
	~SceneValueEditor();

	void AddDebug(const char* name, ModelRender* value);

public:
	void Render();
	void GetSceneValue(struct SceneValue* value);

private:
	void RenderTopMenu();
	void RenderObjectButton();

	void Save();
	void Load();

	class InspectorObject* Add();
	void Remove(class InspectorObject* value);
	void Select(class InspectorObject* value);

private:
	ImVec2 btnSize = ImVec2(180.0f, 24.0f);
	class FileDialogXml* fileDialog;

	class InspectorObject* light;
	class InspectorObject* mainCamera;

	class InspectorFactory* factory;
	vector<class InspectorObject*> objects;
	vector<class InspectorObject*> debugObjects;

	class InspectorObject* selectedObj = nullptr;
};

