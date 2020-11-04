#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

class InspectorObject
{
public:
	InspectorObject(class InspectorFactory* factory);
	~InspectorObject();

public:
	void SetTarget(class IInspectorEditor* value, string typeName);
	void Name(const char* value);
	const char* Name();

public:
	void Render();

	void On();
	void Off();
	void Save(Xml::XMLDocument* document, Xml::XMLElement* root);
	void Load(Xml::XMLElement* node);
	void AddSceneValue(struct SceneValue* value);

private:
	class InspectorFactory* factory;

	class IInspectorEditor* target = nullptr;
	class InputTextImGui* inputName;
	string typeName = "None";
};