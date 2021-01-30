#pragma once

class SceneEditor
{
public:
	SceneEditor();
	~SceneEditor();

public:
	void Update();
	void Render();

public:
	void AddValue(string tag, void* value);

private:
	void RenderTopMenu();
	void RenderObjectButton();

private:
	void Save();
	void Load();
	void WriteFile(wstring file);
	void OpenFile(wstring file);

private:
	ImVec2 btnSize = ImVec2(180.0f, 24.0f);

private:
	struct EditorDesc* desc;
	class ObjectEditorList* objList;

	wstring file = L"new scene.scene";
};

