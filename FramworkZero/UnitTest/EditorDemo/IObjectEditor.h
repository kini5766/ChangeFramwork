#pragma once

class IObjectEditor
{
public:
	virtual ~IObjectEditor() {}
	virtual void ImGuiRender() = 0;
	virtual void Update() {};
	virtual void Render() {};
	virtual void On() {}
	virtual void Off() {}

	virtual void Save(class BinaryWriter* w) = 0;
	virtual void Load(class BinaryReader* r) = 0;
};

class ObjectEditor
{
public:
	ObjectEditor(class ObjectEditorFactory* factory, int number);
	~ObjectEditor();

public:
	void Update();
	void Render();
	void ImGuiRender();

	void On();
	void Off();
	void Save(class BinaryWriter* w);
	void Load(class BinaryReader* r);

public:
	void Name(const char* value);
	const char* Name();

private:
	IObjectEditor* target = nullptr;
	class ObjectEditorFactory* factory;
	class ImGuiInputText* inputName;
	string typeName = "None";
};