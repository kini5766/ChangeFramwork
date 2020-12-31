#include "stdafx.h"
#include "IObjectDebuger.h"

#include "ObjectEditorFactory.h"
#include "Utilities/ImGuiInputText.h"

ObjectDebuger::ObjectDebuger(class ObjectEditorFactory* factory, const char* name, string targetTypeName, void * targetObject)
	: factory(factory)
{
	inputName = new ImGuiInputText(28);
	inputName->Text(name);
	target = factory->CreateDebuger(targetTypeName);
	target->SetObject(targetObject);
}

ObjectDebuger::~ObjectDebuger()
{
	SafeDelete(inputName);
	SafeDelete(target);
}

void ObjectDebuger::Render()
{
	ImGui::Begin("Inspector");
	inputName->Render("Name");
	ImGui::Separator();
	target->Render();
	ImGui::End();
}

void ObjectDebuger::On()
{
	target->On();
}

void ObjectDebuger::Off()
{
	target->Off();
}

void ObjectDebuger::Name(const char * value)
{
	inputName->Text(value);
}

const char * ObjectDebuger::Name()
{
	return inputName->Text();
}