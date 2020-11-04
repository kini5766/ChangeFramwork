#include "Framework.h"
#include "InspectorObject.h"

#include "IInspectorEditor.h"
#include "InspectorFactory.h"
#include "Utilities/Xml.h"
#include "../Utilities/InputTextImGui.h"

InspectorObject::InspectorObject(InspectorFactory * factory)
	: factory(factory)
{
	inputName = new InputTextImGui(28);
	inputName->Text("Object");
}

InspectorObject::~InspectorObject()
{
	SafeDelete(inputName);
	SafeDelete(target);
}

void InspectorObject::SetTarget(IInspectorEditor * value, string _typeName)
{
	SafeDelete(target);

	target = value;
	typeName = _typeName;
}

void InspectorObject::Name(const char * value)
{
	inputName->Text(value);
}

const char * InspectorObject::Name()
{
	return inputName->Text();
}

void InspectorObject::On()
{
	if (target)
		target->On();
}

void InspectorObject::Off()
{
	if (target)
		target->Off();
}

void InspectorObject::Render()
{
	ImGui::Begin("Inspector");
	inputName->Render("Name");
	ImGui::Separator();

	if (target)
	{
		target->Render();
		ImGui::End();
		return;
	}

	UINT size;
	string* names = factory->GetNames(&size);
	for (UINT i = 0; i < size; i++)
	{
		if (ImGui::Button(names[i].c_str()))
		{
			target = factory->CreateEditor(names[i]);
			typeName = names[i];
			On();
		}
	}

	ImGui::End();
}

void InspectorObject::Save(Xml::XMLDocument * document, Xml::XMLElement * root)
{
	// <InspectorObject>
	Xml::XMLElement* node = document->NewElement("InspectorObject");
	node->SetAttribute("Type", typeName.c_str());
	Xml::XMLElement* child;

	// <Name>
	child = document->NewElement("Name");
	child->SetText(inputName->Text());
	node->LinkEndChild(child);
	// </Name>

	if (target)
	{
		// <Values>
		child = document->NewElement("Values");
		target->Save(document, child);
		node->LinkEndChild(child);
		// </Values>
	}

	root->LinkEndChild(node);
	// </InspectorObject>
}

void InspectorObject::Load(Xml::XMLElement * node)
{
	const char* text;
	text = node->Attribute("Type");
	typeName = text;

	Xml::XMLElement* child = node->FirstChildElement();

	// <Name>
	inputName->Text(child->GetText());
	// </Name>

	child = child->NextSiblingElement();

	SafeDelete(target);
	target = factory->CreateEditor(typeName);
	if (target)
	{
		// <Values>
		target->Load(child);
		// </Values>
	}
}

void InspectorObject::AddSceneValue(SceneValue * value)
{
	if (nullptr == target)
		return;

	target->AddSceneValue(value);
}
