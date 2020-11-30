#include "Framework.h"
#include "InspectorCube.h"

InspectorCube::InspectorCube(Shader * shader)
	: shader(shader)
{
	cube = new MeshCube(shader);
}

InspectorCube::~InspectorCube()
{
	SafeDelete(cube);
}

void InspectorCube::On()
{
	pass = 1;
}

void InspectorCube::Off()
{
	pass = 0;
	cube->Pass(pass);
}

void InspectorCube::AddSceneValue(SceneValue * value)
{
	if (nullptr == cube)
		return;

	value->RenderObjs.push_back(cube);
}

void InspectorCube::Render()
{
	ImGui::SliderInt("Pass", (int*)&pass, 0, 1);
	cube->Pass(pass);

	Vector3 float3Value;
	cube->GetTransform()->Position(&float3Value);
	ImGui::DragFloat3("Position", float3Value, 0.1f);
	cube->GetTransform()->Position(float3Value);
	cube->GetTransform()->RotationDegree(&float3Value);
	ImGui::DragFloat3("Rotation", float3Value, 0.9f);
	cube->GetTransform()->RotationDegree(float3Value);
	cube->GetTransform()->Scale(&float3Value);
	ImGui::DragFloat3("Scale", float3Value, 0.01f);
	cube->GetTransform()->Scale(float3Value);

}

void InspectorCube::Save(Xml::XMLDocument * document, Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child;

	// <Position>
	child = document->NewElement("Position");
	cube->GetTransform()->Position(&float3Value);
	child->SetAttribute("x", float3Value.x);
	child->SetAttribute("y", float3Value.y);
	child->SetAttribute("z", float3Value.z);
	node->LinkEndChild(child);
	// </Position>

	// <Rotation>
	child = document->NewElement("Rotation");
	cube->GetTransform()->RotationDegree(&float3Value);
	child->SetAttribute("x", float3Value.x);
	child->SetAttribute("y", float3Value.y);
	child->SetAttribute("z", float3Value.z);
	node->LinkEndChild(child);
	// </Rotation>

	// <Scale>
	child = document->NewElement("Scale");
	cube->GetTransform()->Scale(&float3Value);
	child->SetAttribute("x", float3Value.x);
	child->SetAttribute("y", float3Value.y);
	child->SetAttribute("z", float3Value.z);
	node->LinkEndChild(child);
	// </Scale>

	node->LinkEndChild(child);

}

void InspectorCube::Load(Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child = node->FirstChildElement();

	// <Position>
	float3Value.x = child->FloatAttribute("x");
	float3Value.y = child->FloatAttribute("y");
	float3Value.z = child->FloatAttribute("z");
	cube->GetTransform()->Position(float3Value);
	// </Position>

	child = child->NextSiblingElement();

	// <Rotation>
	float3Value.x = child->FloatAttribute("x");
	float3Value.y = child->FloatAttribute("y");
	float3Value.z = child->FloatAttribute("z");
	cube->GetTransform()->RotationDegree(float3Value);
	// </Rotation>

	child = child->NextSiblingElement();

	// <Scale>
	float3Value.x = child->FloatAttribute("x");
	float3Value.y = child->FloatAttribute("y");
	float3Value.z = child->FloatAttribute("z");
	cube->GetTransform()->Scale(float3Value);
	// </Scale>

}
