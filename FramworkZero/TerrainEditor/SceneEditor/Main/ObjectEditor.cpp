#include "stdafx.h"
#include "ObjectEditor.h"

#include "Utilities/BinaryFile.h"
#include "Tools/ImGuiInputText.h"
#include "ObjectEditorFactory.h"


ObjectEditor::ObjectEditor(EditorDesc * desc)
	: desc(desc)
{
	inputName = new ImGuiInputText(28);
	inputName->Text("Object ");
}

ObjectEditor::~ObjectEditor()
{
	SafeDelete(inputName);
	SafeDelete(target);
}

void ObjectEditor::ImGuiRender()
{
	ImGui::Begin("Inspector");
	inputName->Render("Name");
	ImGui::Separator();

	if (target != nullptr)
	{
		target->ImGuiRender();
		ImGui::End();
		return;
	}

	// 수정하고 있는 오브젝트가 없는 경우
	if (ImGui::CollapsingHeader("Select Object Type", ImGuiTreeNodeFlags_DefaultOpen))
	{
		UINT size;
		string* names = desc->Factory->GetNames(&size);
		for (UINT i = 0; i < size; i++)
		{
			if (ImGui::Button(names[i].c_str()))
			{
				typeName = names[i];
				target = desc->Factory->CreateEditor(typeName);
				target->Initialize(desc->Scene);
				On();
			}
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

void ObjectEditor::Save(BinaryWriter * w)
{
	w->String(typeName);
	w->String(inputName->Text());

	if (target)
	{
		target->Save(w);
	}
	
}

void ObjectEditor::Load(BinaryReader * r)
{
	typeName = r->String();
	inputName->Text(r->String().c_str());

	SafeDelete(target);
	if (typeName != "None")
	{
		target = desc->Factory->CreateEditor(typeName);
		assert(target != nullptr);
		target->Load(r);
		target->Initialize(desc->Scene);
	}
}

void ObjectEditor::Name(const char * value)
{
	inputName->Text(value);
}

const char * ObjectEditor::Name()
{
	return inputName->Text();
}