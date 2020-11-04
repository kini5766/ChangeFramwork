#pragma once

#include "../IInspectorEditor.h"

class InspectorLight : public IInspectorEditor
{
public:
	InspectorLight();
	~InspectorLight();

public:
	// IInspectorEditor을(를) 통해 상속됨
	void Render() override;
	void Save(Xml::XMLDocument* document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;
};



class InspectorMainCamera : public IInspectorEditor
{
public:
	InspectorMainCamera();
	~InspectorMainCamera();

public:
	// IInspectorEditor을(를) 통해 상속됨
	void Render() override;
	void Save(Xml::XMLDocument* document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;
};