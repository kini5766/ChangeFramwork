#pragma once

class SceneEditor
{
public:
	SceneEditor();
	~SceneEditor();

public:
	void Update();
	void Render();

private:
	void RenderTopMenu();
	void RenderObjectButton();
	void RenderSelected();

	class ObjectEditor* CreateEditor();
	//void AddEditor(string tag, void* input);

private:
	void Save();
	void Load();
	void WriteFile(wstring file);
	void OpenFile(wstring file);

private:
	ImVec2 btnSize = ImVec2(180.0f, 24.0f);

	class SceneValue* value;
	class ObjectEditorFactory* factory;

	wstring file = L"new scene.scene";
};

