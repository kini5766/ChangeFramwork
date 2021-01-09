#pragma once

#include "Utilities/BinaryFile.h"

class IObjectEditor
{
public:
	virtual ~IObjectEditor() {}
	virtual void ImGuiRender() = 0;
	virtual void Update() {};
	virtual void Render() {};
	virtual void On() {}
	virtual void Off() {}

	virtual void Save(BinaryWriter* w) = 0;
	virtual void Load(BinaryReader* r) = 0;
	// return : 로드 성공 여부
	virtual bool LoadTakeOut(BinaryReader* r) { Load(r); return true; }
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
	void Save(BinaryWriter* w);
	void Load(BinaryReader* r);
	bool LoadTakeOut(BinaryReader* r);

public:
	void Name(const char* value);
	const char* Name();

private:
	IObjectEditor* target = nullptr;
	class ObjectEditorFactory* factory;
	class ImGuiInputText* inputName;
	string typeName = "None";
};