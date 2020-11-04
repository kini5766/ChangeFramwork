#include "Framework.h"
#include "InspectorContext.h"

// InspectorLight /////////////////////////////////////////////////////////////

InspectorLight::InspectorLight()
{
}

InspectorLight::~InspectorLight()
{
}

void InspectorLight::Render()
{
	ImGui::SliderFloat3("Direction", Context::Get()->Direction(), -1.0f, 1.0f);
}

void InspectorLight::Save(Xml::XMLDocument* document, Xml::XMLElement * node)
{
	// <Direction>
	Xml::XMLElement* child = document->NewElement("Direction");
	child->SetAttribute("x", Context::Get()->Direction().x);
	child->SetAttribute("y", Context::Get()->Direction().y);
	child->SetAttribute("z", Context::Get()->Direction().z);
	node->LinkEndChild(child);
	// </Direction>
}

void InspectorLight::Load(Xml::XMLElement * node)
{
	// <Direction>
	Xml::XMLElement* child = node->FirstChildElement();
	Context::Get()->Direction().x = child->FloatAttribute("x");
	Context::Get()->Direction().y = child->FloatAttribute("y");
	Context::Get()->Direction().z = child->FloatAttribute("z");
	// </Direction>
}


// InspectorMainCamera /////////////////////////////////////////////////////////////

InspectorMainCamera::InspectorMainCamera()
{
}

InspectorMainCamera::~InspectorMainCamera()
{
}

void InspectorMainCamera::Render()
{
	Vector3 float3Value;
	Context::Get()->GetCamera()->Position(&float3Value);
	ImGui::DragFloat3("Position", float3Value, 0.1f);
	Context::Get()->GetCamera()->Position(float3Value);
	Context::Get()->GetCamera()->RotationDegree(&float3Value);
	ImGui::DragFloat3("Rotation", float3Value, 0.1f);
	Context::Get()->GetCamera()->RotationDegree(float3Value);
}

void InspectorMainCamera::Save(Xml::XMLDocument * document, Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child;

	// <Position>
	child = document->NewElement("Position");
	Context::Get()->GetCamera()->Position(&float3Value);
	child->SetAttribute("x", float3Value.x);
	child->SetAttribute("y", float3Value.y);
	child->SetAttribute("z", float3Value.z);
	node->LinkEndChild(child);
	// </Position>

	// <Rotation>
	child = document->NewElement("Rotation");
	Context::Get()->GetCamera()->RotationDegree(&float3Value);
	child->SetAttribute("x", float3Value.x);
	child->SetAttribute("y", float3Value.y);
	child->SetAttribute("z", float3Value.z);
	node->LinkEndChild(child);
	// </Rotation>

	// </MainCamera>
}

void InspectorMainCamera::Load(Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child = node->FirstChildElement();

	// <Position>
	float3Value.x = child->FloatAttribute("x");
	float3Value.y = child->FloatAttribute("y");
	float3Value.z = child->FloatAttribute("z");
	Context::Get()->GetCamera()->Position(float3Value);
	// </Position>

	child = child->NextSiblingElement();

	// <Rotation>
	float3Value.x = child->FloatAttribute("x");
	float3Value.y = child->FloatAttribute("y");
	float3Value.z = child->FloatAttribute("z");
	Context::Get()->GetCamera()->RotationDegree(float3Value);
	// </Rotation>
}
