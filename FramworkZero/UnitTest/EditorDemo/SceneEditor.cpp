#include "stdafx.h"
#include "SceneEditor.h"

#include "Utilities/BinaryFile.h"
#include "ObjectEditorFactory.h"
#include "SceneValues.h"
#include "IObjectEditor.h"
#include "IObjectDebuger.h"

SceneEditor::SceneEditor()
{
	factory = new ObjectEditorFactory();
	sceneValue = new SceneValues();

	factory->InsertDebuger();
}

SceneEditor::~SceneEditor()
{
	for (auto d : objects)
		SafeDelete(d);

	for (auto d : debugObjects)
		SafeDelete(d);

	SafeDelete(factory);
	SafeDelete(sceneValue);
}

void SceneEditor::Update()
{
	for (auto obj : sceneValue->Objs)
		obj->Update();
}

void SceneEditor::Render()
{
	ImGui::Begin("Editor");
	RenderTopMenu();
	ImGui::Separator();
	RenderObjectButton();
	ImGui::End();

	RenderSelectedEditor();

	for (auto obj : sceneValue->Objs)
		obj->Render();
}

void SceneEditor::AddDebug(const char* objectName, const char * typeName, void * targetObject)
{
	ObjectDebuger* debuger = new ObjectDebuger(factory, objectName, typeName, targetObject);
	debugObjects.push_back(debuger);
}

void SceneEditor::RenderTopMenu()
{
	if (ImGui::Button("Save")) Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Load();
	if (ImGui::Button("SelectNone")) SelectNone();
	if (ImGui::Button("CreateObject")) CreateEditor();
	if (selectedType == SelectedType::Editor)
	{
		if (ImGui::Button("DeleteObject")) DestoryEditor(selectedNum);
	}
	else
	{
		ImGui::NewLine();
		ImGui::NewLine();
	}
}

void SceneEditor::RenderObjectButton()
{
	ImGui::LabelText("", "Debug Objects");
	UINT sizeDebug = debugObjects.size();
	for (UINT i = 0; i < sizeDebug; i++)
	{
		ImGui::PushID(i);
		if (ImGui::Button(debugObjects[i]->Name(), btnSize))
			SelectDebug(i);
		ImGui::PopID();
	}

	//ImGui::NewLine();
	//ImGui::NewLine();
	ImGui::Separator();

	ImGui::LabelText("", "Edit Objects");
	UINT size = objects.size();
	for (UINT i = 0; i < size; i++)
	{
		ImGui::PushID(sizeDebug + i);
		if (ImGui::Button(objects[i]->Name(), btnSize))
			Select(i);
		ImGui::PopID();
	}
}

void SceneEditor::RenderSelectedEditor()
{
	if (selectedType == SelectedType::Editor)
		objects[selectedNum]->Render();
	if (selectedType == SelectedType::Debug)
		debugObjects[selectedNum]->Render();
}

void SceneEditor::CreateEditor()
{
	ObjectEditor* obj = new ObjectEditor(factory, static_cast<int>(objects.size()), sceneValue);
	objects.push_back(obj);
}

void SceneEditor::DestoryEditor(UINT index)
{
	vector<ObjectEditor*>::iterator iter = objects.begin() + index;
	SafeDelete(*iter);
	objects.erase(iter);
	SelectNone();
}

void SceneEditor::Deselect()
{
	if (selectedType == SelectedType::Editor)
		objects[selectedNum]->Off();
	else if (selectedType == SelectedType::Debug)
		debugObjects[selectedNum]->Off();
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
	objects[index]->On();
}

void SceneEditor::SelectDebug(UINT index)
{
	if (selectedType == SelectedType::Debug && index == selectedNum)
	{
		SelectNone();
		return;
	}
	Deselect();
	selectedType = SelectedType::Debug;
	selectedNum = index;
	debugObjects[index]->On();
}

void SceneEditor::Save()
{
	//Path::SaveFileDialog();
}

void SceneEditor::Load()
{
	//Path::OpenFileDialog();
}
