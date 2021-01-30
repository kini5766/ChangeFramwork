#include "stdafx.h"
#include "SceneEditor.h"
#include "Utilities/BinaryFile.h"

#include "ObjectEditorFactory.h"
#include "ObjectEditorList.h"
#include "E_SceneValue.h"
#include "ObjectEditor.h"

SceneEditor::SceneEditor()
{
	desc->Factory = new ObjectEditorFactory();
	desc->Scene = new E_SceneValue();
	objList = new ObjectEditorList();
}

SceneEditor::~SceneEditor()
{
	SafeDelete(objList);
	SafeDelete(desc->Scene);
	SafeDelete(desc->Factory);
	SafeDelete(desc);
}

void SceneEditor::Update()
{
	desc->Scene->Update();
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

	if (objList->IsSelected())
		objList->GetObj(objList->GetSelected())->ImGuiRender();

	desc->Scene->Render();
}

void SceneEditor::AddValue(string tag, void * value)
{
	desc->Scene->AddValue(tag, value);
}


#pragma region Render ImGui

void SceneEditor::RenderTopMenu()
{
	if (ImGui::Button("Save")) Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Load();
	if (ImGui::Button("SelectNone")) objList->SelectNone();
	if (ImGui::Button("CreateObject")) objList->CreateEditor(desc);
	if (objList->IsSelected())
	{
		if (ImGui::Button("DeleteObject"))
			objList->Destroy(objList->GetSelected());
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
		UINT size = objList->Size();
		for (UINT i = 0; i < size; i++)
		{
			ImGui::PushID(i);
			string imguiName = objList->GetObj(i)->Name();
			if (objList->GetSelected() == i)
				imguiName = ">" + imguiName + "<";

			if (ImGui::Button(imguiName.c_str(), btnSize))
				objList->Select(i);
			ImGui::PopID();
		}
	}
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

	UINT size = objList->Size();
	w.UInt(size);

	for (UINT i = 0; i < size; i++)
		objList->GetObj(i)->Save(&w);
}

void SceneEditor::OpenFile(wstring file)
{
	objList->SelectNone();
	objList->Clear();

	BinaryReader r(file);

	UINT size = r.UInt();

	for (UINT i = 0; i < size; i++)
	{
		ObjectEditor* obj = objList->CreateEditor(desc);
		obj->Load(&r);
	}
}

#pragma endregion

