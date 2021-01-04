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

SceneValue * SceneEditor::Takeout(wstring file)
{
	SceneValue * result;

	OpenFile(URI::Scenes + file + L".scene");

	result = value;
	value = nullptr;

	return result;
}

void SceneEditor::Update()
{
	value->Update();
}

void SceneEditor::Render()
{
	ImGui::Begin("Editor");
	//ImGui::DockSpace(ImGui::GetID("Editor"));
	RenderTopMenu();
	ImGui::Separator();
	RenderObjectButton();
	ImGui::End();

	RenderSelected();

	value->Render();
}


#pragma region Render ImGui

void SceneEditor::RenderTopMenu()
{
	if (ImGui::Button("Save")) Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Load();
	if (ImGui::Button("SelectNone")) SelectNone();
	if (ImGui::Button("CreateObject")) CreateEditor();
	if (selectedType == SelectedType::Editor)
	{
		if (ImGui::Button("DeleteObject"))
		{
			UINT num = selectedNum;
			SelectNone();
			value->Destroy(num);
		}
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
			string imguiName = value->Obj(i)->Name();
			if (selectedNum == i)
				imguiName = ">" + imguiName + "<";

			if (ImGui::Button(imguiName.c_str(), btnSize))
				Select(i);
			ImGui::PopID();
		}
	}
}

void SceneEditor::RenderSelected()
{
	if (selectedType == SelectedType::Editor)
		value->Obj(selectedNum)->ImGuiRender();
}

#pragma endregion


#pragma region Select

void SceneEditor::Deselect()
{
	if (selectedType == SelectedType::Editor)
		value->Obj(selectedNum)->Off();
}

void SceneEditor::SelectNone()
{
	Deselect();
	selectedType = SelectedType::None;
	selectedNum = -1;
}

void SceneEditor::Select(UINT index)
{
	if (selectedType == SelectedType::Editor && selectedNum == index)
	{
		SelectNone();
		return;
	}
	Deselect();
	selectedType = SelectedType::Editor;
	selectedNum = index;
	value->Obj(selectedNum)->On();
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
		value->Obj(i)->Save(&w);
}

void SceneEditor::OpenFile(wstring file)
{
	SelectNone();
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


