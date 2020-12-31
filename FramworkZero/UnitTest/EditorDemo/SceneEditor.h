#pragma once

class SceneEditor
{
public:
	SceneEditor();
	~SceneEditor();

public:
	void Update();
	void Render();
	void AddDebug(const char* objectName, const char* typeName, void* targetObject = nullptr);

private:
	void RenderTopMenu();
	void RenderObjectButton();
	void RenderSelectedEditor();

	void CreateEditor();
	void DestoryEditor(UINT index);

	void Deselect();
	void SelectNone();
	void Select(UINT index);
	void SelectDebug(UINT index);

private:
	void Save();
	void Load();

private:
	ImVec2 btnSize = ImVec2(180.0f, 24.0f);

	struct SceneValues* sceneValue;
	class ObjectEditorFactory* factory;
	vector<class ObjectEditor*> objects;
	vector<class ObjectDebuger*> debugObjects;

	UINT selectedNum = 0;

private:
	enum class SelectedType { 
		None = 0, Editor, Debug 
	} selectedType;
};

