#include "stdafx.h"
#include "IObjectEditor.h"

#include "Utilities/ImGuiInputText.h"
#include "Utilities/BinaryFile.h"
#include "ObjectEditorFactory.h"
#include "SceneValues.h"

ObjectEditor::ObjectEditor(ObjectEditorFactory * factory, int number, SceneValues* targetScene)
	: factory(factory), targetScene(targetScene)
{
	inputName = new ImGuiInputText(28);
	inputName->Text(("Object " + to_string(number)).c_str());
}

ObjectEditor::~ObjectEditor()
{
	SafeDelete(inputName);
	SafeDelete(target);
}

void ObjectEditor::Render()
{
	ImGui::Begin("Inspector");
	inputName->Render("Name");
	ImGui::Separator();

	if (target != nullptr)
	{
		target->Render();
		ImGui::End();
		return;
	}

	// 수정하고 있는 오브젝트가 없는 경우
	UINT size;
	string* names = factory->GetNames(&size);
	for (UINT i = 0; i < size; i++)
	{
		if (ImGui::Button(names[i].c_str()))
		{
			typeName = names[i];
			target = factory->CreateEditor(typeName);
			target->Initialize(targetScene);
			On();
		}
	}

	ImGui::End();
}

void ObjectEditor::On()
{
	if (target)
		target->On();
}

void ObjectEditor::Off()
{
	if (target)
		target->Off();
}


void ObjectEditor::Name(const char * value)
{
	inputName->Text(value);
}

const char * ObjectEditor::Name()
{
	return inputName->Text();
}