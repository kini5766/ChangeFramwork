#pragma once

#include "IObjectEditor.h"

struct EditorDesc
{
	class ObjectEditorFactory* Factory;
	class E_SceneValue* Scene;
};

class ObjectEditor
{
public:
	ObjectEditor(EditorDesc* desc);
	~ObjectEditor();

public:
	void ImGuiRender();

	void On();
	void Off();
	void Save(BinaryWriter* w);
	void Load(BinaryReader* r);

public:
	void Name(const char* value);
	const char* Name();

private:
	EditorDesc* desc;
	IObjectEditor* target = nullptr;
	class ImGuiInputText* inputName;
	string typeName = "None";
};