#pragma once

class IObjectEditor
{
public:
	virtual ~IObjectEditor() {}
	virtual void Render() = 0;
	virtual void On() {}
	virtual void Off() {}

	virtual void Initialize(struct SceneValues* targetScene) = 0;
	virtual void Save(class BinaryWriter* w) = 0;
	virtual void Load(class BinaryReader* r) = 0;
};

class ObjectEditor
{
public:
	ObjectEditor(class ObjectEditorFactory* factory, int number, struct SceneValues* targetScene);
	~ObjectEditor();

public:
	void Render();

	void On();
	void Off();
	void Save(class BinaryWriter* w) {}
	void Load(class BinaryReader* r) {}

public:
	void Name(const char* value);
	const char* Name();

private:
	IObjectEditor* target = nullptr;
	struct SceneValues* targetScene;
	class ObjectEditorFactory* factory;
	class ImGuiInputText* inputName;
	string typeName = "None";
};