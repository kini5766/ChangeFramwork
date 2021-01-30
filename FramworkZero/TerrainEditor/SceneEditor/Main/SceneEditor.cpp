#include "stdafx.h"
#include "SceneEditor.h"

#include "Utilities/BinaryFile.h"
#include "ObjectEditorFactory.h"
#include "SceneValue.h"
#include "IObjectEditor.h"

SceneEditor::SceneEditor()
{
	factory = new ObjectEditorFactory();
	value = new SceneValue();
}

SceneEditor::~SceneEditor()
{
	SafeDelete(value);
	SafeDelete(factory);
}

void SceneEditor::Update()
{
	value->Update();
}

void SceneEditor::Render()
{
	//ImGui::Begin("Scene");
	//ImGui::DockSpace(ImGui::GetID("Scene"));
	//ImGui::End();
	{
		ImGui::Begin("Editor");
		RenderTopMenu();
		ImGui::Separator();
		RenderObjectButton();
		ImGui::End();
	}
	RenderSelected();

	value->Render();
}


#pragma region Render ImGui

void SceneEditor::RenderTopMenu()
{
	if (ImGui::Button("Save")) Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Load();
	if (ImGui::Button("SelectNone")) value->SelectNone();
	if (ImGui::Button("CreateObject")) CreateEditor();
	if (value->IsSelected())
	{
		if (ImGui::Button("DeleteObject"))
			value->Destroy(value->GetSelected());

	}
	else
	{
		ImGui::NewLine();
		ImGui::Spacing();
		ImGui::Spacing();
	}
}

void SceneEditor::RenderObjectButton()
{
	if (ImGui::CollapsingHeader("Edit Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		UINT size = value->Size();
		for (UINT i = 0; i < size; i++)
		{
			ImGui::PushID(i);
			string imguiName = value->GetObj(i)->Name();
			if (value->GetSelected() == i)
				imguiName = ">" + imguiName + "<";

			if (ImGui::Button(imguiName.c_str(), btnSize))
				value->Select(i);
			ImGui::PopID();
		}
	}
}

#pragma endregion


#pragma region Object Create

ObjectEditor* SceneEditor::CreateEditor()
{
	ObjectEditor* obj = new ObjectEditor(factory, static_cast<int>(value->Size()));
	value->Add(obj);
	return obj;
}

#pragma endregion


#pragma region File

void SceneEditor::Save()
{
	Path::SaveFileDialog(file,
		L"Scene File\0*.scene", URI::Scenes,
		bind(&SceneEditor::WriteFile, this, placeholders::_1),
		WinDesc::GetHandle()
	);
}

void SceneEditor::Load()
{
	Path::OpenFileDialog(file,
		L"Scene File\0*.scene", URI::Scenes,
		bind(&SceneEditor::OpenFile, this, placeholders::_1),
		WinDesc::GetHandle()
	);
}

void SceneEditor::WriteFile(wstring file)
{
	BinaryWriter w(file);

	UINT size = value->Size();
	w.UInt(size);

	for (UINT i = 0; i < size; i++)
		value->GetObj(i)->Save(&w);
}

void SceneEditor::OpenFile(wstring file)
{
	value->SelectNone();
	value->Clear();

	BinaryReader r(file);

	UINT size = r.UInt();

	for (UINT i = 0; i < size; i++)
	{
		ObjectEditor* obj = CreateEditor();
		obj->Load(&r);
	}
}

#pragma endregion

