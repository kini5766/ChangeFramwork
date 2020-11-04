#include "Framework.h"
#include "InspectorModel.h"

#include "../../Utilities/InputTextImGui.h"
#define INPUT_MAX_COUNT 28

InspectorModel::InspectorModel(Shader* shader)
	: shader(shader)
{
	inputMaterial = new InputTextImGui(INPUT_MAX_COUNT);
	inputMesh = new InputTextImGui(INPUT_MAX_COUNT);
}

InspectorModel::~InspectorModel()
{
	SafeDelete(model);
	SafeDelete(inputMesh);
	SafeDelete(inputMaterial);
}

void InspectorModel::On()
{
	pass = 4;
}

void InspectorModel::Off()
{
	pass = 1;

	if (nullptr == model)
		return;

	model->Pass(pass);
}

void InspectorModel::AddSceneValue(SceneValue * value)
{
	if (nullptr == model)
		return;

	value->ModelObjs.push_back(model);
}

void InspectorModel::Render()
{
	if (model == nullptr)
	{
		if (ImGui::Combo("Model", &item, modelItem))
		{
			inputMaterial->Text(String::ToString(materialItems[item]).c_str());
			inputMesh->Text(String::ToString(meshItems[item]).c_str());
		}

		if (item == 0)
		{
			inputMaterial->Render("Material");
			inputMesh->Render("Mesh");
		}

		if (ImGui::Button("CreateModel"))
			CreateModel();

		return;
	}

	ImGui::SliderInt("Pass", (int*)&pass, 0, 5);

	model->Pass(pass);

	Vector3 float3Value;
	model->GetTransform()->Position(&float3Value);
	ImGui::DragFloat3("Position", float3Value, 0.1f);
	model->GetTransform()->Position(float3Value);
	model->GetTransform()->RotationDegree(&float3Value);
	ImGui::DragFloat3("Rotation", float3Value, 0.9f);
	model->GetTransform()->RotationDegree(float3Value);
	model->GetTransform()->Scale(&float3Value);
	ImGui::DragFloat3("Scale", float3Value, 0.01f);
	model->GetTransform()->Scale(float3Value);
}

void InspectorModel::Save(Xml::XMLDocument * document, Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child;

	// <Material>
	child = document->NewElement("Material");
	child->SetText(inputMaterial->Text());
	node->LinkEndChild(child);
	// </Material>

	// <Mesh>
	child = document->NewElement("Mesh");
	child->SetText(inputMesh->Text());
	node->LinkEndChild(child);
	// </Mesh>

	// <Model>
	if (nullptr != model)
	{
		child = document->NewElement("Model");
		Xml::XMLElement* modelchild;

		// <Position>
		modelchild = document->NewElement("Position");
		model->GetTransform()->Position(&float3Value);
		modelchild->SetAttribute("x", float3Value.x);
		modelchild->SetAttribute("y", float3Value.y);
		modelchild->SetAttribute("z", float3Value.z);
		child->LinkEndChild(modelchild);
		// </Position>

		// <Rotation>
		modelchild = document->NewElement("Rotation");
		model->GetTransform()->RotationDegree(&float3Value);
		modelchild->SetAttribute("x", float3Value.x);
		modelchild->SetAttribute("y", float3Value.y);
		modelchild->SetAttribute("z", float3Value.z);
		child->LinkEndChild(modelchild);
		// </Rotation>

		// <Scale>
		modelchild = document->NewElement("Scale");
		model->GetTransform()->Scale(&float3Value);
		modelchild->SetAttribute("x", float3Value.x);
		modelchild->SetAttribute("y", float3Value.y);
		modelchild->SetAttribute("z", float3Value.z);
		child->LinkEndChild(modelchild);
		// </Scale>

		node->LinkEndChild(child);
	}
	// </Model>

}

void InspectorModel::Load(Xml::XMLElement * node)
{
	Xml::XMLElement* child = node->FirstChildElement();
	const char* text;

	// <Material>
	text = child->GetText();
	inputMaterial->Text(text);
	// </Material>

	child = child->NextSiblingElement();

	// <Mesh>
	text = child->GetText();
	inputMesh->Text(text);
	// </Mesh>

	child = child->NextSiblingElement();

	// <Model>
	if (nullptr != child)
	{
		Xml::XMLElement* modelchild = child->FirstChildElement();

		CreateModel();

		Vector3 float3Value;

		// <Position>
		float3Value.x = modelchild->FloatAttribute("x");
		float3Value.y = modelchild->FloatAttribute("y");
		float3Value.z = modelchild->FloatAttribute("z");
		model->GetTransform()->Position(float3Value);
		// </Position>

		modelchild = modelchild->NextSiblingElement();

		// <Rotation>
		float3Value.x = modelchild->FloatAttribute("x");
		float3Value.y = modelchild->FloatAttribute("y");
		float3Value.z = modelchild->FloatAttribute("z");
		model->GetTransform()->RotationDegree(float3Value);
		// </Rotation>

		modelchild = modelchild->NextSiblingElement();

		// <Scale>
		float3Value.x = modelchild->FloatAttribute("x");
		float3Value.y = modelchild->FloatAttribute("y");
		float3Value.z = modelchild->FloatAttribute("z");
		model->GetTransform()->Scale(float3Value);
		// </Scale>
	}
	// </Model>
}

void InspectorModel::CreateModel()
{
	SafeDelete(model);
	model = new ModelRender(shader);
	model->ReadMaterial(String::ToWString(inputMaterial->Text()));
	model->ReadMesh(String::ToWString(inputMesh->Text()));
	model->Pass(pass);
}
