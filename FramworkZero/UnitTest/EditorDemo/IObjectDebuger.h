#pragma once

class IObjectDebuger
{
public:
	virtual ~IObjectDebuger() {}
	virtual void Render() = 0;
	virtual void On() {}
	virtual void Off() {}

	virtual void SetObject(void* value) = 0;
};

class ObjectDebuger
{
public:
	ObjectDebuger(
		class ObjectEditorFactory* factory, 
		const char* name, 
		string targetTypeName, 
		void * targetObject
	);
	~ObjectDebuger();

public:
	void Render();

	void On();
	void Off();

public:
	void Name(const char* value);
	const char* Name();

private:
	IObjectDebuger* target = nullptr;
	class ObjectEditorFactory* factory;
	class ImGuiInputText* inputName;
};