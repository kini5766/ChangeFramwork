#include "Framework.h"
#include "SceneValueEditor.h"
#include "SceneValue.h"
#include "Utilities/FileDialogXml.h"

#include "Inspector/IInspectorEditor.h"
#include "Inspector/InspectorFactory.h"
#include "Inspector/InspectorObject.h"
#include "Inspector/Objects/InspectorContext.h"

SceneValueEditor::SceneValueEditor()
{
	fileDialog = new FileDialogXml();
	factory = new InspectorFactory();

	mainCamera = new InspectorObject(factory);
	light = new InspectorObject(factory);

	mainCamera->SetTarget(new InspectorMainCamera(), "MainCamera");
	light->SetTarget(new InspectorLight(), "DirectionLight");

	mainCamera->Name("Main Camera");
	light->Name("Direction Light");
}

SceneValueEditor::~SceneValueEditor()
{
	for (InspectorObject* obj : objects)
		SafeDelete(obj);

	for (InspectorObject* obj : debugObjects)
		SafeDelete(obj);

	SafeDelete(light);
	SafeDelete(mainCamera);

	SafeDelete(factory);
	SafeDelete(fileDialog);
}

void SceneValueEditor::AddDebug(const char* name, ModelRender * value)
{
	InspectorModel* model = dynamic_cast<InspectorModel*>(factory->CreateEditor("Model"));
	model->Model(value);

	InspectorObject* obj = new InspectorObject(factory);
	obj->SetTarget(model, "Model");
	obj->Name(name);
	debugObjects.push_back(obj);
}

void SceneValueEditor::Render()
{
	RenderTopMenu();
	ImGui::Separator();
	RenderObjectButton();
}

void SceneValueEditor::GetSceneValue(SceneValue* value)
{
	value->Clear();

	for (InspectorObject* obj : objects)
		obj->AddSceneValue(value);

	for (InspectorObject* obj : debugObjects)
		obj->AddSceneValue(value);
}


void SceneValueEditor::RenderTopMenu()
{
	ImGui::Begin("Editor");
	if (ImGui::Button("Save"))
	{
		Save();
		fileDialog->Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		function<void(void)> f = bind(&SceneValueEditor::Load, this);
		fileDialog->Load(f);
	}

	if (ImGui::Button("CreateObject"))
		Add();


	vector<InspectorObject*>::iterator iter;
	for (iter = objects.begin(); iter != objects.end(); iter++)
		if ((*iter) == selectedObj) break;

	if (iter != objects.end())
	{
		if (ImGui::Button("DeleteModel"))
		{
			objects.erase(iter);
			SafeDelete(selectedObj);
			selectedObj = nullptr;
		}
	}
	else
	{
		ImGui::NewLine();
		ImGui::NewLine();
	}
}

void SceneValueEditor::RenderObjectButton()
{
	ImGui::LabelText("", "Edit Objects");
	if (ImGui::Button(light->Name(), btnSize))
		Select(light);

	if (ImGui::Button(mainCamera->Name(), btnSize))
		Select(mainCamera);

	int size = static_cast<int>(objects.size());
	for (int i = 0; i < size; i++)
	{
		ImGui::PushID(i);
		if (ImGui::Button(objects[i]->Name(), btnSize))
			Select(objects[i]);
		ImGui::PopID();
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Separator();

	ImGui::LabelText("", "Debug Objects");
	for (InspectorObject* obj : debugObjects)
	{
		if (ImGui::Button(obj->Name(), btnSize))
			Select(obj);
	}

	ImGui::End();

	if (selectedObj != nullptr)
		selectedObj->Render();
}

void SceneValueEditor::Save()
{
	fileDialog->Clear();
	fileDialog->CreateDecl("SceneValueEditor");

	Xml::XMLDocument* document = fileDialog->Document();
	Xml::XMLElement* root = fileDialog->Root();
	Xml::XMLElement* node;

	// <InspectorStatic Type = "DirectionLight">
	node = document->NewElement("InspectorStatic");
	node->SetAttribute("Type", "DirectionLight");
	light->Save(document, node);
	root->LinkEndChild(node);
	// </InspectorStatic>

	// <InspectorStatic Type = "MainCamera">
	node = document->NewElement("InspectorStatic");
	node->SetAttribute("Type", "MainCamera");
	mainCamera->Save(document, node);
	root->LinkEndChild(node);
	// </InspectorStatic>

	// <InspectorObjects>
	node = document->NewElement("InspectorObjects");
	for (InspectorObject* obj : objects)
		obj->Save(document, node);
	root->LinkEndChild(node);
	// </InspectorObjects>
}

void SceneValueEditor::Load()
{
	selectedObj = nullptr;

	Xml::XMLElement* root = fileDialog->Root();
	Xml::XMLElement* node = root->FirstChildElement();
	Xml::XMLElement* child;

	// <InspectorStatic Type = "DirectionLight">
	child = node->FirstChildElement();
	light->Load(child);
	// </InspectorStatic>

	node = node->NextSiblingElement();

	// <InspectorStatic Type = "MainCamera">
	child = node->FirstChildElement();
	mainCamera->Load(child);
	// </InspectorStatic>

	node = node->NextSiblingElement();

	// <InspectorObjects>
	child = node->FirstChildElement();
	for (InspectorObject* obj : objects)
		SafeDelete(obj);
	objects.clear();
	while (child != nullptr)
	{
		InspectorObject* obj = new InspectorObject(factory);
		obj->Load(child);
		objects.push_back(obj);

		child = child->NextSiblingElement();
	}
	// </InspectorObjects>
}


InspectorObject * SceneValueEditor::Add()
{
	InspectorObject* obj = new InspectorObject(factory);
	objects.push_back(obj);
	return obj;
}

void SceneValueEditor::Remove(InspectorObject * value)
{
	vector<InspectorObject*>::iterator iter;
	for (iter = objects.begin(); iter != objects.end(); iter++)
	{
		if ((*iter) == value)
		{
			objects.erase(iter);
			Select(nullptr);
			SafeDelete(value);
			break;
		}
	}
}

void SceneValueEditor::Select(InspectorObject * value)
{
	if (selectedObj == value)
		value = nullptr;
	
	if (nullptr != selectedObj)
		selectedObj->Off();
	
	selectedObj = value;
	
	if (nullptr != selectedObj)
		selectedObj->On();
}
