#include "Framework.h"
#include "InspectorTerrain.h"
#include "Environment/Terrain.h"

InspectorTerrain::InspectorTerrain(Shader* shader)
	: shader(shader)
{
}

InspectorTerrain::~InspectorTerrain()
{
	SafeDelete(terrain);
}

void InspectorTerrain::On()
{
	pass = 1;
}

void InspectorTerrain::Off()
{
	pass = 0;

	if (nullptr == terrain)
		return;
	terrain->Pass(pass);
}

void InspectorTerrain::AddSceneValue(SceneValue * value)
{
	if (nullptr == terrain)
		return;

	value->RenderObjs.push_back(terrain);
}

void InspectorTerrain::Render()
{
	if (terrain == nullptr)
	{
		ImGui::InputText("HeightMap", heightMap, 28);
		ImGui::InputText("BaseMap", baseMap, 28);

		if (ImGui::Button("CreateModel"))
			CreateTerrain(String::ToWString(heightMap), String::ToWString(baseMap));
		return;
	}

	ImGui::SliderInt("Pass", (int*)&pass, 0, 1);
	terrain->Pass(pass);

	Vector3 float3Value;
	terrain->GetTransform()->Position(&float3Value);
	ImGui::DragFloat3("Position", float3Value, 0.1f);
	terrain->GetTransform()->Position(float3Value);
	terrain->GetTransform()->RotationDegree(&float3Value);
	ImGui::DragFloat3("Rotation", float3Value, 0.9f);
	terrain->GetTransform()->RotationDegree(float3Value);
	terrain->GetTransform()->Scale(&float3Value);
	ImGui::DragFloat3("Scale", float3Value, 0.01f);
	terrain->GetTransform()->Scale(float3Value);
}

void InspectorTerrain::Save(Xml::XMLDocument * document, Xml::XMLElement * node)
{
	Vector3 float3Value;
	Xml::XMLElement* child;

	// <HeightMap>
	child = document->NewElement("HeightMap");
	child->SetText(heightMap);
	node->LinkEndChild(child);
	// </HeightMap>

	// <BaseMap>
	child = document->NewElement("BaseMap");
	child->SetText(baseMap);
	node->LinkEndChild(child);
	// </BaseMap>

	// <Terrain>
	if (nullptr != terrain)
	{
		child = document->NewElement("Terrain");
		Xml::XMLElement* child2;

		// <Position>
		child2 = document->NewElement("Position");
		terrain->GetTransform()->Position(&float3Value);
		child2->SetAttribute("x", float3Value.x);
		child2->SetAttribute("y", float3Value.y);
		child2->SetAttribute("z", float3Value.z);
		child->LinkEndChild(child2);
		// </Position>

		// <Rotation>
		child2 = document->NewElement("Rotation");
		terrain->GetTransform()->RotationDegree(&float3Value);
		child2->SetAttribute("x", float3Value.x);
		child2->SetAttribute("y", float3Value.y);
		child2->SetAttribute("z", float3Value.z);
		child->LinkEndChild(child2);
		// </Rotation>

		// <Scale>
		child2 = document->NewElement("Scale");
		terrain->GetTransform()->Scale(&float3Value);
		child2->SetAttribute("x", float3Value.x);
		child2->SetAttribute("y", float3Value.y);
		child2->SetAttribute("z", float3Value.z);
		child->LinkEndChild(child2);
		// </Scale>

		node->LinkEndChild(child);
	}
	// </Terrain>

}

void InspectorTerrain::Load(Xml::XMLElement * node)
{
	Xml::XMLElement* child = node->FirstChildElement();

	// <HeightMap>
	HeightMap(child->GetText());
	// </HeightMap>

	child = child->NextSiblingElement();

	// <BaseMap>
	BaseMap(child->GetText());
	// </BaseMap>

	child = child->NextSiblingElement();

	// <Terrain>
	if (nullptr != child)
	{
		Xml::XMLElement* child2 = child->FirstChildElement();

		CreateTerrain(String::ToWString(heightMap), String::ToWString(baseMap));

		Vector3 float3Value;

		// <Position>
		float3Value.x = child2->FloatAttribute("x");
		float3Value.y = child2->FloatAttribute("y");
		float3Value.z = child2->FloatAttribute("z");
		terrain->GetTransform()->Position(float3Value);
		// </Position>

		child2 = child2->NextSiblingElement();

		// <Rotation>
		float3Value.x = child2->FloatAttribute("x");
		float3Value.y = child2->FloatAttribute("y");
		float3Value.z = child2->FloatAttribute("z");
		terrain->GetTransform()->RotationDegree(float3Value);
		// </Rotation>

		child2 = child2->NextSiblingElement();

		// <Scale>
		float3Value.x = child2->FloatAttribute("x");
		float3Value.y = child2->FloatAttribute("y");
		float3Value.z = child2->FloatAttribute("z");
		terrain->GetTransform()->Scale(float3Value);
		// </Scale>
	}
	// </Terrain>
}

void InspectorTerrain::CreateTerrain(const wstring& _heightMap, const wstring& _baseMap)
{
	SafeDelete(terrain);
	terrain = new Terrain(shader, _heightMap);
	terrain->BaseMap(_baseMap);
}

void InspectorTerrain::HeightMap(const char * value)
{
	ZeroMemory(heightMap, sizeof(char) * 28);
	for (int i = 0; value[i] != '\0'; i++)
		heightMap[i] = value[i];
}

void InspectorTerrain::BaseMap(const char * value)
{
	ZeroMemory(baseMap, sizeof(char) * 28);
	for (int i = 0; value[i] != '\0'; i++)
		baseMap[i] = value[i];
}
