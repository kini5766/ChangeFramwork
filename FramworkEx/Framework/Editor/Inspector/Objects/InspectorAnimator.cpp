#include "Framework.h"
#include "InspectorAnimator.h"

#include "../../Utilities/InputTextImGui.h"
#define INPUT_MAX_COUNT 28

InspectorAnimator::InspectorAnimator(Shader* shader)
	: shader(shader)
{
	inputClips.push_back(new InputTextImGui(INPUT_MAX_COUNT));
	inputMaterial = new InputTextImGui(INPUT_MAX_COUNT);
	inputMesh = new InputTextImGui(INPUT_MAX_COUNT);
}

InspectorAnimator::~InspectorAnimator()
{
	SafeDelete(model);
	SafeDelete(inputMesh);
	SafeDelete(inputMaterial);

	for (InputTextImGui* d : inputClips)
		SafeDelete(d);
}

void InspectorAnimator::On()
{
	pass = 5;
}

void InspectorAnimator::Off()
{
	pass = 2;

	if (nullptr == model)
		return;

	model->Pass(pass);
}

void InspectorAnimator::AddSceneValue(SceneValue * value)
{
	if (nullptr == model)
		return;

	value->ModelAnimObjs.push_back(model);
}

void InspectorAnimator::Render()
{
	if (model == nullptr)
	{
		if (ImGui::Combo("Model", &item, modelItem))
		{
			inputMaterial->Text(String::ToString(materialItems[item]).c_str());
			inputMesh->Text(String::ToString(meshItems[item]).c_str());
		}

		inputMaterial->Render("Material");
		inputMesh->Render("Mesh");
		RenderClipInputs("Clip ");

		if (ImGui::Button("CreateAnimator"))
		{
			CreateModel();
		}

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

	if (ImGui::Combo("Clip", &clip, clipItem.c_str()))
	{
		model->PlayClip(clip);
	}

}

void InspectorAnimator::Save(Xml::XMLDocument * document, Xml::XMLElement * node)
{
	//Vector3 float3Value;
	//Xml::XMLElement* child;

	//// <Material>
	//child = document->NewElement("Material");
	//child->SetText(inputMaterial->Text());
	//node->LinkEndChild(child);
	//// </Material>

	//// <Mesh>
	//child = document->NewElement("Mesh");
	//child->SetText(inputMesh->Text());
	//node->LinkEndChild(child);
	//// </Mesh>

	//// <Model>
	//if (nullptr != model)
	//{
	//	child = document->NewElement("Model");
	//	Xml::XMLElement* modelchild;

	//	// <Position>
	//	modelchild = document->NewElement("Position");
	//	model->GetTransform()->Position(&float3Value);
	//	modelchild->SetAttribute("x", float3Value.x);
	//	modelchild->SetAttribute("y", float3Value.y);
	//	modelchild->SetAttribute("z", float3Value.z);
	//	child->LinkEndChild(modelchild);
	//	// </Position>

	//	// <Rotation>
	//	modelchild = document->NewElement("Rotation");
	//	model->GetTransform()->RotationDegree(&float3Value);
	//	modelchild->SetAttribute("x", float3Value.x);
	//	modelchild->SetAttribute("y", float3Value.y);
	//	modelchild->SetAttribute("z", float3Value.z);
	//	child->LinkEndChild(modelchild);
	//	// </Rotation>

	//	// <Scale>
	//	modelchild = document->NewElement("Scale");
	//	model->GetTransform()->Scale(&float3Value);
	//	modelchild->SetAttribute("x", float3Value.x);
	//	modelchild->SetAttribute("y", float3Value.y);
	//	modelchild->SetAttribute("z", float3Value.z);
	//	child->LinkEndChild(modelchild);
	//	// </Scale>

	//	node->LinkEndChild(child);
	//}
	//// </Model>

}

void InspectorAnimator::Load(Xml::XMLElement * node)
{
	//Xml::XMLElement* child = node->FirstChildElement();
	//const char* text;

	//// <Material>
	//text = child->GetText();
	//inputMaterial->Text(text);
	//// </Material>

	//child = child->NextSiblingElement();

	//// <Mesh>
	//text = child->GetText();
	//inputMesh->Text(text);
	//// </Mesh>

	//child = child->NextSiblingElement();

	//// <Model>
	//if (nullptr != child)
	//{
	//	Xml::XMLElement* modelchild = child->FirstChildElement();

	//	CreateModel();

	//	Vector3 float3Value;

	//	// <Position>
	//	float3Value.x = modelchild->FloatAttribute("x");
	//	float3Value.y = modelchild->FloatAttribute("y");
	//	float3Value.z = modelchild->FloatAttribute("z");
	//	model->GetTransform()->Position(float3Value);
	//	// </Position>

	//	modelchild = modelchild->NextSiblingElement();

	//	// <Rotation>
	//	float3Value.x = modelchild->FloatAttribute("x");
	//	float3Value.y = modelchild->FloatAttribute("y");
	//	float3Value.z = modelchild->FloatAttribute("z");
	//	model->GetTransform()->RotationDegree(float3Value);
	//	// </Rotation>

	//	modelchild = modelchild->NextSiblingElement();

	//	// <Scale>
	//	float3Value.x = modelchild->FloatAttribute("x");
	//	float3Value.y = modelchild->FloatAttribute("y");
	//	float3Value.z = modelchild->FloatAttribute("z");
	//	model->GetTransform()->Scale(float3Value);
	//	// </Scale>
	//}
	//// </Model>
}

void InspectorAnimator::RenderClipInputs(string value)
{
	string label;
	UINT clipCount = inputClips.size() - 1;
	for (UINT i = 0; i < clipCount; i++)
	{
		if (inputClips[i]->Text()[0] == '\0')
		{
			inputClips.erase(inputClips.begin() + i);
			i--;
			clipCount--;
			continue;
		}
		label = value + to_string(i);
		inputClips[i]->Render(label.c_str());
	}

	label = value + to_string(clipCount);
	inputClips[clipCount]->Render(label.c_str());
	if (inputClips[clipCount]->Text()[0] != '\0')
		inputClips.push_back(new InputTextImGui(INPUT_MAX_COUNT));
}

void InspectorAnimator::CreateModel()
{
	SafeDelete(model);
	model = new ModelAnimator(shader);
	model->ReadMaterial(String::ToWString(inputMaterial->Text()));
	model->ReadMesh(String::ToWString(inputMesh->Text()));

	UINT clipSize = inputClips.size() - 1;
	for (UINT i = 0; i < clipSize; i++)
	{
		model->ReadClip(String::ToWString(inputClips[i]->Text()));
		clipItem += inputClips[i]->Text();
		clipItem += '\0';
	}

	model->Pass(pass);

}

